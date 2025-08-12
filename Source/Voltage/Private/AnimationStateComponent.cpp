// Fill out your copyright notice in the Description page of Project Settings.

#include "AnimationStateComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "CombatComponent.h"
#include "AdvancedMovementComponent.h"
#include "Voltage/Voltage_AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"

UAnimationStateComponent::UAnimationStateComponent() :
	Speed(0.f),
	bIsInAir(false),
	bIsAccelerating(false),
	MovementOffsetYaw(0.f),
	LastMovementOffsetYaw(0.f),
	bAiming(false),
	CharacterRotation(FRotator(0.f)),
	CharacterRotationLastFrame(FRotator(0.f)),
	TIPCharacterYaw(0.f),
	TIPCharacterYawLastFrame(0.f),
	RootYawOffset(0.f),
	Pitch(0.f),
	bReloading(false),
	OffsetState(EOffsetState::EOS_Hip),
	YawDelta(0.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UAnimationStateComponent::BeginPlay()
{
	Super::BeginPlay();
	VoltageCharacter = Cast<AVoltageCharacter>(GetOwner());
	if (VoltageCharacter)
	{
		AdvancedMovementComp = VoltageCharacter->FindComponentByClass<UAdvancedMovementComponent>();
		CombatComp = VoltageCharacter->FindComponentByClass<UCombatComponent>();
		
		if (VoltageCharacter->GetMesh())
		{
			VoltageAnimInstance = Cast<UVoltage_AnimInstance>(VoltageCharacter->GetMesh()->GetAnimInstance());
		}
	}
}

void UAnimationStateComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAnimationStateComponent::UpdateAnimationState(float DeltaTime)
{
	if (VoltageCharacter && AdvancedMovementComp && CombatComp)
	{
		bCrouching = AdvancedMovementComp->GetCrouching();
		bReloading = CombatComp->GetCombatState() == ECombatState::ECS_Reloading;
		
		FVector Velocity{ VoltageCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();

		bIsInAir = VoltageCharacter->GetCharacterMovement()->IsFalling();
 
		if (VoltageCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
		{
			bIsAccelerating = true;
		}
		else
		{
			bIsAccelerating = false;
		}
		
		FRotator AimRotation = VoltageCharacter->GetBaseAimRotation();
		FRotator MovementRotation = UKismetMathLibrary::MakeRotFromX(
			VoltageCharacter->GetVelocity());
		MovementOffsetYaw = UKismetMathLibrary::NormalizedDeltaRotator(
			MovementRotation,
			AimRotation).Yaw;

		if (VoltageCharacter->GetVelocity().Size() > 0.f)
		{
			LastMovementOffsetYaw = MovementOffsetYaw;
		}
		
		bAiming	= CombatComp->GetAiming();

		if (bReloading)
		{
			OffsetState = EOffsetState::EOS_Reloading;
		}
		else if (bIsInAir)
		{
			OffsetState = EOffsetState::EOS_InAir;
		}
		else if (CombatComp->GetAiming())
		{
			OffsetState = EOffsetState::EOS_Aiming;
		}
		else
		{
			OffsetState = EOffsetState::EOS_Hip;
		}
		if (CombatComp->GetEquippedWeapon())
		{
			//EquippedWeaponType = VoltageCharacter->GetEquippedWeapon()->GetWeaponType();
		}
	}
	TurnInPlace();
	Lean(DeltaTime);
}


void UAnimationStateComponent::TurnInPlace()
{
	if (VoltageCharacter == nullptr) return;

	Pitch = VoltageCharacter->GetBaseAimRotation().Pitch;
	
	if (Speed > 0 || bIsInAir)
	{
		RootYawOffset = 0.f;
		TIPCharacterYaw = VoltageCharacter->GetActorRotation().Yaw;
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		RotationCurveLastFrame = 0.f;
		RotationCurve = 0.f;
	}	
	else
	{
		TIPCharacterYawLastFrame = TIPCharacterYaw;
		TIPCharacterYaw = VoltageCharacter->GetActorRotation().Yaw;
		const float TIPYawDelta{ TIPCharacterYaw - TIPCharacterYawLastFrame };

		// Debug Turning Side
		/*if (FMath::Abs(TIPYawDelta) > 0.1f)
		{
			if (TIPYawDelta > 0.f)
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Turn Right"));
			}
			else
			{
				GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Blue, TEXT("Turn Left"));
			}
		}*/
		
		RootYawOffset = UKismetMathLibrary::NormalizeAxis(RootYawOffset - TIPYawDelta);
		/*UAnimInstance* AnimInstance = VoltageCharacter->GetMesh()->GetAnimInstance();*/
		
		const float Turning{ VoltageAnimInstance->GetCurveValue(TEXT("Turning")) };
		if (Turning > 0)
		{
			RotationCurveLastFrame =  RotationCurve;
			RotationCurve = VoltageAnimInstance->GetCurveValue(TEXT("Rotation"));
			const float DeltaRotation{ RotationCurve - RotationCurveLastFrame };
			
			RootYawOffset > 0 ? RootYawOffset -= DeltaRotation : RootYawOffset += DeltaRotation;

			const float ABSRootYawOffset{ FMath::Abs(RootYawOffset) };
			if (ABSRootYawOffset > 90.f)
			{
				const float YawExcess{ ABSRootYawOffset - 90.f };
				RootYawOffset > 0 ? RootYawOffset -= YawExcess : RootYawOffset += YawExcess;
			}
		}
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, -1, FColor::Red, FString::Printf(TEXT("RootYawOffset: %f"), RootYawOffset));
}

void UAnimationStateComponent::Lean(float DeltaTime)
{
	if (VoltageCharacter == nullptr) return;
	CharacterRotationLastFrame = CharacterRotation;
	CharacterRotation = VoltageCharacter->GetActorRotation();
	
	const FRotator Delta{ UKismetMathLibrary::NormalizedDeltaRotator(CharacterRotation, CharacterRotationLastFrame) };
	
	const float Target = Delta.Yaw / DeltaTime;
	const float Interp{ FMath::FInterpTo(YawDelta, Target, DeltaTime, 6.f) };
	YawDelta = FMath::Clamp(Interp, -90.f, 90.f);
}


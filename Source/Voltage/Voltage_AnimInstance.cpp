// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltage_AnimInstance.h"
#include "Voltage_Character.h"
#include "AnimationStateComponent.h"

UVoltage_AnimInstance::UVoltage_AnimInstance() :
	RootYawOffset(0.f),
	Pitch(0.f)
{
}

void UVoltage_AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwningCharacter = Cast<AVoltageCharacter>(TryGetPawnOwner());
	if (OwningCharacter)
	{
		CombatComponent = OwningCharacter->GetCombatComponent();
	}
}

void UVoltage_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!AnimationStateComponent)
	{
		OwningCharacter = Cast<AVoltageCharacter>(TryGetPawnOwner());
		if (OwningCharacter)
		{
			AnimationStateComponent = OwningCharacter->FindComponentByClass<UAnimationStateComponent>();
		}
	}

	if (!AnimationStateComponent) return;

	AnimationStateComponent->UpdateAnimationState(DeltaSeconds);

	Speed = AnimationStateComponent->Speed;
	bIsInAir = AnimationStateComponent->bIsInAir;
	bIsAccelerating = AnimationStateComponent->bIsAccelerating;
	bCrouching = AnimationStateComponent->bCrouching;
	bReloading = AnimationStateComponent->bReloading;
	bAiming = AnimationStateComponent->bAiming;
	OffsetState = AnimationStateComponent->OffsetState;
	MovementOffsetYaw = AnimationStateComponent->MovementOffsetYaw;
	LastMovementOffsetYaw = AnimationStateComponent->LastMovementOffsetYaw;
	RootYawOffset = AnimationStateComponent->RootYawOffset;
	Pitch = AnimationStateComponent->Pitch;
}
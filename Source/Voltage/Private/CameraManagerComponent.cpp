// Fill out your copyright notice in the Description page of Project Settings.

#include "CameraManagerComponent.h"
#include "Voltage/Voltage_Character.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

UCameraManagerComponent::UCameraManagerComponent() :
	bAiming(false),
	CameraDefaultFOV(90.f),
	CameraZoomedFOV(60.f),
	CameraCurrentFOV(90.f),
	ZoomInterpSpeed(20.f)
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	VoltageChar = Cast<AVoltageCharacter>(GetOwner());
}

void UCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

    CameraInterpZoom(DeltaTime);

    /*GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, FString::Printf(TEXT("FOV: %f"), CameraCurrentFOV));*/
}

void UCameraManagerComponent::Move(const FInputActionValue& Value)
{
	AActor* Owner = GetOwner();
	if (!Owner) return;

	ACharacter* Character = Cast<ACharacter>(Owner);
	if (!Character) return;

	FVector2D Input = Value.Get<FVector2D>();
	if (Input.SizeSquared() <= 0.f) return;

	AController* Controller = Character->GetController();
	if (!Controller) return;

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	Character->AddMovementInput(Forward, Input.Y);
	Character->AddMovementInput(Right, Input.X);
}

void UCameraManagerComponent::Look(const FInputActionValue& Value)
{
	if (APawn* Pawn = Cast<APawn>(GetOwner()))
	{
		if (AController* Controller = Pawn->GetController())
		{
			FVector2D LookInput = Value.Get<FVector2D>();

			if (APlayerController* PC = Cast<APlayerController>(Controller))
			{
				PC->AddYawInput(LookInput.X * MouseSensitivity);
				PC->AddPitchInput(-LookInput.Y * MouseSensitivity);
			}
		}
	}
}

void UCameraManagerComponent::CameraInterpZoom(float DeltaTime)
{
	if (bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraZoomedFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(
			CameraCurrentFOV,
			CameraDefaultFOV,
			DeltaTime,
			ZoomInterpSpeed);
	}

	if (VoltageChar && VoltageChar->Camera)
	{
		VoltageChar->Camera->SetFieldOfView(CameraCurrentFOV);
	}
}

void UCameraManagerComponent::SetAiming(bool bNewAiming)
{
	bAiming = bNewAiming;
}

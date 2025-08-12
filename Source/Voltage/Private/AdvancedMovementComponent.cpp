// Fill out your copyright notice in the Description page of Project Settings.

#include "AdvancedMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CapsuleComponent.h"

UAdvancedMovementComponent::UAdvancedMovementComponent() :
	BaseTurnRate(45.f),
	BaseLookUpRate(45.f),
	BaseMovementSpeed(650.f),
	CrouchMovementSpeed(300.f),
	StandingCapsuleHalfHeight(88.f),
	CrouchingCapsuleHalfHeight(44.f),
	BaseGroundFriction(2.f),
	CrouchingGroundFriction(100.f),
	bCrouching(false),
	OwnerCharacter(nullptr),
	CharacterMovement(nullptr),
	CapsuleComponent(nullptr),
	MeshComponent(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UAdvancedMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(LogTemp, Error, TEXT("AdvancedMovementComponent: Owner is not a character!"));
		return;
	}

	CharacterMovement = OwnerCharacter->GetCharacterMovement();
	CapsuleComponent = OwnerCharacter->GetCapsuleComponent();
	MeshComponent = OwnerCharacter->GetMesh();

	// Safe check
	if (!CharacterMovement || !CapsuleComponent || !MeshComponent) return;

	// Set controller rotation usage (only valid on ACharacter)
	OwnerCharacter->bUseControllerRotationPitch = false;
	OwnerCharacter->bUseControllerRotationYaw = true;
	OwnerCharacter->bUseControllerRotationRoll = false;

	// Character movement settings
	CharacterMovement->bOrientRotationToMovement = false;
	CharacterMovement->RotationRate = FRotator(0.f, 540.f, 0.f);
	CharacterMovement->JumpZVelocity = 600.f;
	CharacterMovement->AirControl = 0.2f;
}


// Called every frame
void UAdvancedMovementComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	InterpCapsuleHalfHeight(DeltaTime);
}

void UAdvancedMovementComponent::Jump()
{
	if (!OwnerCharacter) return;

	UCharacterMovementComponent* MovementComponent = OwnerCharacter->GetCharacterMovement();
	if (!MovementComponent) return;

	if (bCrouching)
	{
		bCrouching = false;
		MovementComponent->MaxWalkSpeed = BaseMovementSpeed;
	}
	else
	{
		OwnerCharacter->Jump(); // виклик Jump() з ACharacter
	}
}

void UAdvancedMovementComponent::CrouchButtonPressed()
{
	if (!CharacterMovement || CharacterMovement->IsFalling())
		return;

	bCrouching = !bCrouching;

	if (bCrouching)
	{
		CharacterMovement->MaxWalkSpeed = CrouchMovementSpeed;
		CharacterMovement->GroundFriction = CrouchingGroundFriction;
	}
	else
	{
		CharacterMovement->MaxWalkSpeed = BaseMovementSpeed;
		CharacterMovement->GroundFriction = BaseGroundFriction;
	}
}

void UAdvancedMovementComponent::InterpCapsuleHalfHeight(float DeltaTime)
{
	if (!IsValid(CapsuleComponent) || !IsValid(MeshComponent))
		return;

	float TargetCapsuleHalfHeight = bCrouching ? CrouchingCapsuleHalfHeight : StandingCapsuleHalfHeight;
	
	const float InterpHalfHeight = FMath::FInterpTo(
		CapsuleComponent->GetScaledCapsuleHalfHeight(),
		TargetCapsuleHalfHeight,
		DeltaTime,
		20.f);

	const float DeltaCapsuleHalfHeight = InterpHalfHeight - CapsuleComponent->GetScaledCapsuleHalfHeight();
	const FVector MeshOffset(0.f, 0.f, -DeltaCapsuleHalfHeight);
	MeshComponent->AddLocalOffset(MeshOffset);
	
	CapsuleComponent->SetCapsuleHalfHeight(InterpHalfHeight);
}


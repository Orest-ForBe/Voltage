// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterInputHandler.h"
#include "Voltage/Voltage_Character.h"
#include "EnhancedInputComponent.h"
#include "AdvancedMovementComponent.h"
#include "CameraManagerComponent.h"
#include "CombatComponent.h"

// Sets default values for this component's properties
UCharacterInputHandler::UCharacterInputHandler()
{
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UCharacterInputHandler::BeginPlay()
{
	Super::BeginPlay();
	InitializeOwnerRefs();
}


// Called every frame
void UCharacterInputHandler::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UCharacterInputHandler::InitializeOwnerRefs()
{
	OwnerCharacter = Cast<AVoltageCharacter>(GetOwner());
	if (OwnerCharacter)
	{
		CombatComp = OwnerCharacter->FindComponentByClass<UCombatComponent>();
	}
}

void UCharacterInputHandler::SetupInputBindings(UEnhancedInputComponent* InputComponent)
{
	InitializeOwnerRefs();
	
	if (!OwnerCharacter || !InputComponent) return;

	// Base Move Input
	UCameraManagerComponent* CameraComp = OwnerCharacter->CameraComp;
	if (CameraComp)
	{
		InputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, CameraComp, &UCameraManagerComponent::Move);	
		InputComponent->BindAction(LookAction, ETriggerEvent::Triggered, CameraComp, &UCameraManagerComponent::Look);
	}
    
	// Combat
	InputComponent->BindAction(FireAction, ETriggerEvent::Started, CombatComp, &UCombatComponent::FireButtonPressed);
	InputComponent->BindAction(FireAction, ETriggerEvent::Completed, CombatComp, &UCombatComponent::FireButtonReleased);
	InputComponent->BindAction(AimingAction, ETriggerEvent::Started, CombatComp, &UCombatComponent::AimingButtonPressed);
	InputComponent->BindAction(AimingAction, ETriggerEvent::Completed, CombatComp, &UCombatComponent::AimingButtonReleased);
	InputComponent->BindAction(SwitchShootingTypeAction, ETriggerEvent::Completed, CombatComp, &UCombatComponent::SwitchShootingType);
	InputComponent->BindAction(WeaponReloadAction, ETriggerEvent::Completed, CombatComp, &UCombatComponent::ReloadButtonPressed);

	// Movement abilities
	InputComponent->BindAction(JumpAction, ETriggerEvent::Started, OwnerCharacter, &ACharacter::Jump);
	InputComponent->BindAction(JumpAction, ETriggerEvent::Completed, OwnerCharacter, &ACharacter::StopJumping);

	// If MovementComp exists on the character, bind crouch
	if (UAdvancedMovementComponent* MovementComp = OwnerCharacter->GetAdvancedMovementComponent())
	{
		InputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, MovementComp, &UAdvancedMovementComponent::CrouchButtonPressed);
	}
}
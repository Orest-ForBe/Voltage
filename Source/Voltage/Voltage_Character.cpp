
// Fill out your copyright notice in the Description page of Project Settings.

#include "Voltage_Character.h"
#include "CombatComponent.h"
#include "CameraManagerComponent.h"
#include "AnimationStateComponent.h"
#include "AdvancedMovementComponent.h"
#include "CharacterInputHandler.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "DrawDebugHelpers.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// Sets default values
AVoltageCharacter::AVoltageCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->TargetArmLength = 300.f;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->SocketOffset = FVector(0, 90.f, 70.f);
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true ;
	bUseControllerRotationRoll = false;
	
	AnimStateComp = CreateDefaultSubobject<UAnimationStateComponent>(TEXT("AnimationStateComponent"));

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));
	CombatComp->SetIsReplicated(true); // If using multiplayer

	CameraComp = CreateDefaultSubobject<UCameraManagerComponent>(TEXT("CameraManagerComponent"));
	AdvancedMovementComp = CreateDefaultSubobject<UAdvancedMovementComponent>(TEXT("AdvancedMovementComponent"));
	InputHandlerComp = CreateDefaultSubobject<UCharacterInputHandler>(TEXT("InputHandler"));
}

// Called when the game starts or when spawned
void AVoltageCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (GetMesh() && GetMesh()->GetAttachParent())
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh Attach Parent: %s"), *GetMesh()->GetAttachParent()->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Mesh or Attach Parent is null!"));
	}
	
	if (CombatComp)
	{
		CombatComp->SetCharacterMesh(GetMesh());
	}
	
	// Add Input Mapping Context
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		PC->bShowMouseCursor = false;
		PC->SetInputMode(FInputModeGameOnly()); // <-- ОБОВ'ЯЗКОВО
		
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer()))
		{
			if (InputHandlerComp && InputHandlerComp->DefaultMappingContext)
			{
				Subsystem->AddMappingContext(InputHandlerComp->DefaultMappingContext, 0);
			}
			else
			{
				if (GEngine)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Mapping Context not assigned!"));
				}
			}
		}
	}
	CombatComp->EquipWeapon(CombatComp->SpawnDefaultWeapon());
	CombatComp->InitializeAmmoMap();
	GetCharacterMovement()->MaxWalkSpeed = AdvancedMovementComp->BaseMovementSpeed;
}

// Called every frame
void AVoltageCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CombatComp->ApplyAutoRecoil();
	AdvancedMovementComp->InterpCapsuleHalfHeight(DeltaTime);
}

// Called to bind functionality to input
void AVoltageCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (InputHandlerComp)
		{
			InputHandlerComp->SetupInputBindings(EnhancedInputComponent);
		}
	}
}

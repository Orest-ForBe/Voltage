// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class AWeapon;
class UCombatComponent;
class UAdvancedMovementComponent;
class UCameraManagerComponent;
class UAnimationStateComponent;
class UCharacterInputHandler;

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Voltage_Character.generated.h"

UCLASS()
class VOLTAGE_API AVoltageCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AVoltageCharacter();
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Camera Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	USpringArmComponent* SpringArm;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* Camera;
	
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCombatComponent* CombatComp;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAdvancedMovementComponent* AdvancedMovementComp;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCameraManagerComponent* CameraComp;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UAnimationStateComponent* AnimStateComp;
    
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	UCharacterInputHandler* InputHandlerComp;

	FORCEINLINE UAdvancedMovementComponent* GetAdvancedMovementComponent() const { return AdvancedMovementComp; }
	
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE UCameraComponent* GetFollowCamera() const { return Camera; }
	FORCEINLINE UCombatComponent* GetCombatComponent() const { return CombatComp; }
};
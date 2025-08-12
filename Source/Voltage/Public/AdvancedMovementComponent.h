// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AdvancedMovementComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOLTAGE_API UAdvancedMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAdvancedMovementComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseTurnRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float BaseLookUpRate;

	UFUNCTION()
	void Jump();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	bool bCrouching;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchMovementSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float StandingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchingCapsuleHalfHeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float BaseGroundFriction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
	float CrouchingGroundFriction;

	void CrouchButtonPressed();
	void InterpCapsuleHalfHeight(float DeltaTime);
	
	UPROPERTY()
	ACharacter* OwnerCharacter;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;

	UPROPERTY()
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY()
	USkeletalMeshComponent* MeshComponent;

	FORCEINLINE bool GetCrouching() const { return bCrouching; }
};

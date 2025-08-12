// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InputActionValue.h"
#include "CameraManagerComponent.generated.h"

class AVoltageCharacter;
class USpringArmComponent;
class UCameraComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOLTAGE_API UCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCameraManagerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;;

	// Actor Components
	UPROPERTY()
	AVoltageCharacter* VoltageChar;
	
	// Move to AdvancedMovementComp
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Impact", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	// Mouse Handling
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MouseSensitivity = 1.0f;
	
	// Move from character
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float CameraDefaultFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float CameraZoomedFOV;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input", meta = (AllowPrivateAccess = "true"))
	float CameraCurrentFOV;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	float ZoomInterpSpeed;
    
	// Move these functions
	void CameraInterpZoom(float DeltaTime);

	void SetAiming(bool bNewAiming);

	FORCEINLINE bool GetAiming() const { return bAiming; }
};

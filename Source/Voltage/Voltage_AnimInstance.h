// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

class UAnimationStateComponent;

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Voltage_Character.h"
#include "AnimationStateComponent.h"
#include "Voltage_AnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class VOLTAGE_API UVoltage_AnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UVoltage_AnimInstance();
	
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	AVoltageCharacter* VoltageCharacter;

	UPROPERTY(BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	UCombatComponent* CombatComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Character", meta = (AllowPrivateAccess = "true"))
	AVoltageCharacter* OwningCharacter;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimationStateComponent* AnimationStateComponent;

	UPROPERTY(BlueprintReadOnly, Category = "TurnInPlace", meta = (AllowPrivateAccess = "true"))
	float RootYawOffset;

	UPROPERTY(BlueprintReadOnly, Category = "TurnInPlace", meta = (AllowPrivateAccess = "true"))
	float Pitch;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bCrouching;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bReloading;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	bool bAiming;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	EOffsetState OffsetState;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	UPROPERTY(BlueprintReadOnly, Category = "Movement", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;
};

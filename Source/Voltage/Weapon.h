// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "WeaponType.h"
#include "AmmoType.h"
#include "Sound/SoundCue.h"
#include "Weapon.generated.h"

/**
 * 
 */
UCLASS()
class VOLTAGE_API AWeapon : public AItem
{
	GENERATED_BODY()
public:
	AWeapon();

	virtual void Tick(float DeltaTime) override;
private:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int32 MaxCapacityAmmo;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int32 Ammo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	int32 MagazineCapacity;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	EAmmoType AmmoType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	FName ReloadMontageSection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	bool bMovingClip;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon Properties", meta = (AllowPrivateAccess = true))
	FName ClipBoneName;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundCue* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = true))
	UAnimMontage* FireMontage;

	UPROPERTY(EditAnywhere, Category = "Weapon")
	USoundCue* EmptyMagazineSound;
	
public:
	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMagazineCapacity() const { return MaxCapacityAmmo; }
	
	void DecrementAmmo();
	
	FORCEINLINE EWeaponType GetWeaponType() const { return WeaponType; }
	FORCEINLINE EAmmoType GetAmmoType() const { return AmmoType; }
	FORCEINLINE FName GetReloadMontageSection() const { return ReloadMontageSection; }
	FORCEINLINE FName GetClipBoneName() const { return ClipBoneName; }
	
	void ReloadAmmo(int32 Amount);
	bool ClipIsFull();

	FORCEINLINE void SetMovingClip(bool Move) { bMovingClip = Move; }
	FORCEINLINE USoundCue* GetFireSound() const { return FireSound; }
	FORCEINLINE UAnimMontage* GetFireMontage() const { return FireMontage; }
	FORCEINLINE USoundCue* GetEmptyMagazineSound() const { return EmptyMagazineSound; }
	
};

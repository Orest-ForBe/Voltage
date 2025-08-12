// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"

AWeapon::AWeapon() :
	Ammo(360),
	MagazineCapacity(90),
	WeaponType(EWeaponType::EWT_Secondary),
	AmmoType(EAmmoType::EAT_556),
	ReloadMontageSection(FName(TEXT("Reload"))),
	ClipBoneName(TEXT("ar_clip"))
{
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::DecrementAmmo()
{
	if (Ammo - 1 <= 0)
	{
		Ammo = 0;
	}
	else
	{
		--Ammo;
	}
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Ammo is Enough!"));
	Ammo += Amount;
}

bool AWeapon::ClipIsFull()
{
	return Ammo >= MaxCapacityAmmo;
}

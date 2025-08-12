// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Voltage/Weapon.h"
#include "Voltage/Voltage_Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"
#include "Engine/World.h"
#include "TimerManager.h"
#include "Sound/SoundCue.h"
#include "Particles/ParticleSystemComponent.h"
#include "BulletHitInterface.h"
#include "GameFramework/Character.h"
#include "AdvancedMovementComponent.h"
#include "CameraManagerComponent.h"
#include "Materials/MaterialInterface.h"
#include "Components/DecalComponent.h"
#include "GameFramework/Controller.h"

// Sets default values for this component's properties
UCombatComponent::UCombatComponent() :
	bAiming(false),
	bFireButtonPressed(false),
	bFireShooting(false),
	AutomaticFireRate(0.25f),
	BurstFireRate(0.25f),
	Iterations(3),
	CurrentIteration(0),
	CombatState(ECombatState::ECS_Unoccupied),
	ShootingType(EShootingType::EST_Burst),
	bRecoilRecovering(false),
	RecoilPitch(0.0f),
	MaxRecoilPitch(7.0f),
	RecoilYawOffset(1.5f),
	RecoilYawRandom(0.0f),
	RecoilSmoothSpeed(12.0f),
	RecoilRecoverTime(0.0f),
	RecoilExtraDuration(0.2f),
	RecoilRecoverStrength(8.0f),
	bShouldFire(true),
	CheckAutoFireRate(true),
	Starting_556_Ammo(100),
	Starting_762_Ammo(100),
	Starting_919_Ammo(100)
{
	PrimaryComponentTick.bCanEverTick = true;

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComp"));
}

// Called when the game starts
void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	// Get Camera Manager
	AActor* Owner = GetOwner();
	if (Owner)
	{
		AVoltageCharacter* VoltageChar = Cast<AVoltageCharacter>(Owner);
		if (VoltageChar)
		{
			CameraManagerComp = VoltageChar->FindComponentByClass<UCameraManagerComponent>();
		}
	}

	if (!CameraManagerComp)
	{
		UE_LOG(LogTemp, Error, TEXT("CameraManagerComp not found in CombatComponent!"));
	}
	
	// Get character mesh
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		CharacterMeshComponent = Character->GetMesh();
	}

	InitializeAmmoMap();
    
	if (DefaultWeaponClass)
	{
		AWeapon* StartingWeapon = SpawnDefaultWeapon();
		if (StartingWeapon)
		{
			EquipWeapon(StartingWeapon);
		}
	}
}

void UCombatComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


// Called every frame
void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    
	ApplyAutoRecoil();
}

void UCombatComponent::FireWeapon()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		EquippedWeapon->DecrementAmmo();
	}
	else
	{
		PlayEmptyMagazineSound();
	}
}

void UCombatComponent::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon == nullptr) return;
	if (!CharacterMeshComponent) return;
    
	if (CarryingAmmo() && !EquippedWeapon->ClipIsFull())
	{
		if (bAiming)
		{
			StopAiming();
		}

		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{       
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(
				EquippedWeapon->GetReloadMontageSection());
		}
	}
}

void UCombatComponent::ApplyAutoRecoil()
{
	if (!WeaponHasAmmo()) return;

	AController* Controller = GetOwner() ? GetOwner()->GetInstigatorController() : nullptr;
	if (!Controller || CombatState != ECombatState::ECS_Unoccupied) return;

	if (bFireButtonPressed)
	{
		// Накопичуємо віддачу (приціл піднімається)
		if (RecoilPitch < MaxRecoilPitch)
		{
			RecoilPitch += 1.2f;
		}

		RecoilYawRandom = FMath::FRandRange(-RecoilYawOffset, RecoilYawOffset);

		AddControllerPitchInput(-RecoilPitch * GetWorld()->DeltaTimeSeconds * RecoilSmoothSpeed);
		AddControllerYawInput(RecoilYawRandom * GetWorld()->DeltaTimeSeconds * RecoilSmoothSpeed);

		bRecoilRecovering = true;
		RecoilRecoverTime = 0.f;
	}
	else if (bRecoilRecovering)
	{
		RecoilRecoverTime += GetWorld()->DeltaTimeSeconds;

		if (RecoilRecoverTime <= RecoilExtraDuration)
		{
			// Плавно зменшуємо віддачу і повертаємо приціл вниз
			// Застосовуємо позитивне зміщення (щоб приціл опускався вниз)
			AddControllerPitchInput(RecoilRecoverStrength * GetWorld()->DeltaTimeSeconds);

			// Поступово зменшуємо накопичену віддачу
			RecoilPitch = FMath::Max(0.f, RecoilPitch - (RecoilRecoverStrength * GetWorld()->DeltaTimeSeconds));
		}
		else
		{
			bRecoilRecovering = false;
			RecoilPitch = 0.f;
		}
	}
}

void UCombatComponent::BurstFire()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	CurrentIteration++;

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		EquippedWeapon->DecrementAmmo();
	}
	else
	{
		// Empty Magazine Sound 
		PlayEmptyMagazineSound();
	}

	bFireShooting = true;
	
	if (CurrentIteration >= Iterations)
	{
		GetWorld()->GetTimerManager().ClearTimer(BurstFireTimer);
		bFireShooting = false;
	}
}

void UCombatComponent::TappingFire()
{
	if (EquippedWeapon == nullptr) return;
	if (CombatState != ECombatState::ECS_Unoccupied) return;

	if (WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunFireMontage();
		EquippedWeapon->DecrementAmmo();
	}
	else
	{
		// Empty Magazine Sound 
		PlayEmptyMagazineSound();
	}
}

void UCombatComponent::CheckShootingType()
{
	switch (ShootingType)
	{
	case EShootingType::EST_Burst:
		if (!bFireShooting)
		{
			CurrentIteration = 0;
			GetWorld()->GetTimerManager().SetTimer(
				BurstFireTimer, 
				this, 
				&UCombatComponent::BurstFire, 
				BurstFireRate, 
				true, 
				0.0f);
		}
		break;

	case EShootingType::EST_Semi:
		if (!bFireShooting)
		{
			TappingFire();
		}
		break;
	}

}

void UCombatComponent::PlayFireSound()
{
	if (EquippedWeapon && EquippedWeapon->GetFireSound())
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetFireSound());
	}
}

void UCombatComponent::SendBullet()
{
	if (EquippedWeapon == nullptr) return;
	
	// Send bullet
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");

	if (BarrelSocket)
	{
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

		if (MuzzleFlash)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlash, SocketTransform);
		}

		FVector BeamEnd;
		FHitResult HitDecal;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamEnd, HitDecal);

		if (bBeamEnd)
		{
			if (HitDecal.GetActor())
			{
				IBulletHitInterface* BulletHitInterface = Cast<IBulletHitInterface>(HitDecal.GetActor());
				if (BulletHitInterface)
				{
					BulletHitInterface->BulletHit_Implementation(HitDecal);
				}
			}
			else
			{
				// Spawn default particles
				if (ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(
						GetWorld(),
						ImpactParticles,
						BeamEnd);
				}
			}

			FRotator ImpactRotation = BeamEnd.Rotation();
			FVector ImpactLocation = BeamEnd;
			AActor* ImpactActor;
			ImpactActor = GetWorld()->SpawnActor<AActor>(DestructionImpact, ImpactLocation, ImpactRotation);

			UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(
				GetWorld(),
				BeamParticles,
				SocketTransform);

			if (Beam)
			{
				Beam->SetVectorParameter(FName("Target"), BeamEnd);
			}
			
			// Spawn Decal in TraceHit
			FVector DecalSize(4.0f, 4.0f, 4.0f);
			FRotator DecalRotation = HitDecal.ImpactNormal.Rotation();
			FVector DecalLocation = BeamEnd;

			AActor* Actor = Cast<AActor>(HitDecal.GetActor());

			switch (UGameplayStatics::GetSurfaceType(HitDecal))
			{
				case EPhysicalSurface::SurfaceType1:
					BeamDecal = UGameplayStatics::SpawnDecalAtLocation(Actor, ConcreteHoleDecal, DecalSize, DecalLocation, DecalRotation, 30.f);
					// Attach to *Actor
					break;

				case EPhysicalSurface::SurfaceType2:
					BeamDecal = UGameplayStatics::SpawnDecalAtLocation(Actor, MetalHoleDecal, DecalSize, DecalLocation, DecalRotation);
					break;

				case EPhysicalSurface::SurfaceType4:
					BeamDecal = UGameplayStatics::SpawnDecalAtLocation(Actor, GlassHoleDecal, DecalSize, DecalLocation, DecalRotation);
					break;

				default:
					BeamDecal = UGameplayStatics::SpawnDecalAtLocation(Actor, ConcreteHoleDecal, {10.0f, 10.0f, 10.0f}, DecalLocation, DecalRotation);
					break;
			}

			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, *UEnum::GetValueAsString(UGameplayStatics::GetSurfaceType(HitDecal)));
			/*{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("NoSurfaceType")));
			}*/

			BeamDecal->SetFadeScreenSize(0.f);
		}
	}
}

void UCombatComponent::PlayGunFireMontage()
{
	if (!CharacterMeshComponent) return;
    
	UAnimInstance* AnimInstance = CharacterMeshComponent->GetAnimInstance();
	if (AnimInstance && EquippedWeapon && EquippedWeapon->GetFireMontage())
	{
		AnimInstance->Montage_Play(EquippedWeapon->GetFireMontage());
	}
}

void UCombatComponent::ReloadButtonPressed()
{
	StartReload();
}

void UCombatComponent::PlayReloadSound()
{
	if (FireSound)
	{
		UGameplayStatics::PlaySound2D(this, FireSound);
	}
}

bool UCombatComponent::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;

	auto AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void UCombatComponent::ReleaseClip()
{
	EquippedWeapon->SetMovingClip(false);
}

void UCombatComponent::PlayEmptyMagazineSound()
{
	if (EquippedWeapon && EquippedWeapon->GetEmptyMagazineSound())
	{
		UGameplayStatics::PlaySound2D(this, EquippedWeapon->GetEmptyMagazineSound());
	}
}

bool UCombatComponent::CarryingAmmo() const
{
	if (EquippedWeapon == nullptr) return false;
    
	auto AmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}
	return false;
}

void UCombatComponent::Aim()
{
	bAiming = true;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AdvancedMovementComp)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = AdvancedMovementComp->CrouchMovementSpeed;
	}
	// HUDOverlaySetVisibility(ESlateVisibility::Visible);
}

void UCombatComponent::StopAiming()
{
	bAiming = false;

	ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter && AdvancedMovementComp && !AdvancedMovementComp->bCrouching)
	{
		OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = AdvancedMovementComp->BaseMovementSpeed;
		//HUDOverlay->SetVisibility(ESlateVisibility::Hidden);
	}
}

bool UCombatComponent::GetAiming() const
{
	return bAiming;
}

void UCombatComponent::GrabClip()
{
	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;

	AVoltageCharacter* OwnerCharacter = Cast<AVoltageCharacter>(GetOwner());
	if (!OwnerCharacter) return;
	
	USkeletalMeshComponent* Mesh = OwnerCharacter->GetMesh();
	if (!Mesh) return;
	
	// Index for the clip bone on the Equipped Weapon
	int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName()) };
	// Store the transform of the clip
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	
	HandSceneComponent->AttachToComponent(Mesh, AttachmentRules, FName(TEXT("hand_l")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void UCombatComponent::AddControllerPitchInput(float Value)
{
	if (AController* Controller = GetOwner()->GetInstigatorController())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			PlayerController->AddPitchInput(Value);
		}
	}
}

void UCombatComponent::AddControllerYawInput(float Value)
{
	if (AController* Controller = GetOwner()->GetInstigatorController())
	{
		if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
		{
			PlayerController->AddYawInput(Value);
		}
	}
}

void UCombatComponent::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_556, 120);
	AmmoMap.Add(EAmmoType::EAT_762, 60);
	AmmoMap.Add(EAmmoType::EAT_919, 30);
}

void UCombatComponent::SwitchShootingType()
{
	SwitchFireMode();
}

bool UCombatComponent::TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation)
{
	// Get Viewport Size
	FVector2D ViewportSize;
	if (GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}

	// Get screen space location of crosshairs
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;

	// Get world position and direction of crosshairs
	bool bScreenToWorld = UGameplayStatics::DeprojectScreenToWorld(
		UGameplayStatics::GetPlayerController(this, 0),
		CrosshairLocation,
		CrosshairWorldPosition,
		CrosshairWorldDirection);

	if (bScreenToWorld)
	{
		// Trace from Crosshair world location outward
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 100'000.f };
		OutHitLocation = End;
		GetWorld()->LineTraceSingleByChannel(
			OutHitResult,
			Start,
			End,
			ECollisionChannel::ECC_Visibility);
		if (OutHitResult.bBlockingHit)
		{
			OutHitLocation = OutHitResult.Location;
			return true;
		}
	}
	return false;
}

bool UCombatComponent::GetBeamEndLocation(
	const FVector& MuzzleSocketLocation,
	FVector& OutBeamLocation,
	FHitResult& WeaponTraceHit)
{
	FHitResult CrosshairHitResult;
	bool bCrosshairHit = TraceUnderCrosshairs(CrosshairHitResult, OutBeamLocation);

	if (bCrosshairHit)
	{
		OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{
	}
	
	const FVector WeaponTraceStart{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - WeaponTraceStart };
	const FVector WeaponTraceEnd{ MuzzleSocketLocation + StartToEnd * 1.25f };

	FCollisionResponseParams ResponseParams;
	FCollisionQueryParams QueryParams = FCollisionQueryParams::DefaultQueryParam;
	FActorSpawnParameters SpawnParams;
	QueryParams.bReturnPhysicalMaterial = true;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->LineTraceSingleByChannel(WeaponTraceHit, WeaponTraceStart, WeaponTraceEnd, ECollisionChannel::ECC_Visibility, QueryParams, ResponseParams);

	if (!WeaponTraceHit.bBlockingHit)
	{
		WeaponTraceHit.Location = OutBeamLocation;
		DrawDebugLine(GetWorld(), WeaponTraceStart, OutBeamLocation, FColor::Red, false, 10.f);
		DrawDebugPoint(GetWorld(), WeaponTraceStart, 5.f, FColor::Yellow, false, 30.f);
		DrawDebugPoint(GetWorld(), WeaponTraceStart + StartToEnd, 5.f, FColor::Blue, false, 30.f);
		return false;
	}
	return true;
}

void UCombatComponent::FireButtonPressed()
{
	GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Green, TEXT("Fire Button Pressed"));
	
	bFireButtonPressed = true;
	StartFiring();
}

void UCombatComponent::FireButtonReleased()
{
	bFireButtonPressed = false;
	StopFiring();
}

void UCombatComponent::StartFireTimer()
{
	CombatState = ECombatState::ECS_FireTimerInProgress;
	
	GetWorld()->GetTimerManager().SetTimer(
		AutoFireTimer,
		this,
		&UCombatComponent::AutoFireReset,
		AutomaticFireRate);
}

void UCombatComponent::AimingButtonPressed()
{
	bAiming = true;
	CameraManagerComp->SetAiming(true);
}

void UCombatComponent::AimingButtonReleased()
{
	bAiming = false;
	CameraManagerComp->SetAiming(false);
	StopAiming();
}

AWeapon* UCombatComponent::SpawnDefaultWeapon()
{
	if (!DefaultWeaponClass || !GetWorld()) return nullptr;
    
	return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
}

void UCombatComponent::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (!WeaponToEquip) return;
    
	EquippedWeapon = WeaponToEquip;
	EquippedWeapon->AttachToComponent(
		CharacterMeshComponent,
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		FName("WeaponSocket")
	);
}

bool UCombatComponent::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr) return false;
	return EquippedWeapon && EquippedWeapon->GetAmmo() > 0;
}

void UCombatComponent::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (!EquippedWeapon) return;

	auto AmmoType = EquippedWeapon->GetAmmoType();
	if (AmmoMap.Contains(AmmoType))
	{
		int32& CarriedAmmo = AmmoMap[AmmoType];
		int32 MagEmptySpace = EquippedWeapon->GetMagazineCapacity() - EquippedWeapon->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
		}
		else
		{
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
		}
	}
}

void UCombatComponent::StartFiring()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
    
	bFireButtonPressed = true;
    
	switch (ShootingType)
	{
	case EShootingType::EST_Auto:
		HandleAutoFire();
		break;
	case EShootingType::EST_Burst:
	case EShootingType::EST_Semi:
		CheckShootingType();
		break;
	}
}

void UCombatComponent::StopFiring()
{
	bFireButtonPressed = false;
	bFireShooting = false;
    
	if (ShootingType == EShootingType::EST_Auto)
	{
		GetWorld()->GetTimerManager().ClearTimer(AutoFireTimer);
		CombatState = ECombatState::ECS_Unoccupied;
	}
}

void UCombatComponent::ToggleAiming()
{
	bAiming = !bAiming;
}

void UCombatComponent::StartReload()
{
	ReloadWeapon();
}

void UCombatComponent::SwitchFireMode()
{
	switch (ShootingType)
	{
	case EShootingType::EST_Semi:
		ShootingType = EShootingType::EST_Burst;
		break;
	case EShootingType::EST_Burst:
		ShootingType = EShootingType::EST_Auto;
		break;
	case EShootingType::EST_Auto:
		ShootingType = EShootingType::EST_Semi;
		break;
	}
}

void UCombatComponent::HandleAutoFire()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(AutoFireTimer))
	{
		FireWeapon();
		
		GetWorld()->GetTimerManager().SetTimer(
			AutoFireTimer,
			this,
			&UCombatComponent::FireWeapon,
			AutomaticFireRate,
			true);
	}
}

void UCombatComponent::HandleBurstFire()
{
}

void UCombatComponent::HandleSemiFire()
{
}

void UCombatComponent::AutoFireReset()
{
	CombatState = ECombatState::ECS_Unoccupied;

	if (WeaponHasAmmo() && bFireButtonPressed)
	{
		CheckShootingType();
	}
	else
	{
		PlayEmptyMagazineSound();
	}
}

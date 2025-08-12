// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Voltage/CombatTypes.h"
#include "ShootingType.h"
#include "AmmoType.h"
#include "CombatComponent.generated.h"

class AVoltageCharacter;
class UAdvancedMovementComponent;
class AWeapon;
class IBulletHitInterface;
class UParticleSystem;
class USoundCue;
class USkeletalMeshComponent;
class UCameraManagerComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class VOLTAGE_API UCombatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UCombatComponent();

protected:
    virtual void BeginPlay() override;
    virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:    
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY()
	UAdvancedMovementComponent* AdvancedMovementComp;

	UPROPERTY()
	UCameraManagerComponent* CameraManagerComp;
	
    // Combat State
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
    ECombatState CombatState;
    
    // Core Functions
    void StartFiring();
    void StopFiring();
    void ToggleAiming();
    void StartReload();
    void SwitchFireMode();

    // Weapon Functions
    UFUNCTION(BlueprintCallable)
    void FinishReloading();

    void SetCharacterMesh(USkeletalMeshComponent* Mesh) { CharacterMeshComponent = Mesh; }
    
    // Firing Mechanics
    void FireWeapon();
    void ReloadWeapon();
    void ApplyAutoRecoil();
    void BurstFire();
    void TappingFire();
    void CheckShootingType();

    // Helper Functions
    bool WeaponHasAmmo();
    bool CarryingAmmo() const;
    void PlayEmptyMagazineSound();

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impact, meta = (AllowPrivateAccess = "true"))
    USceneComponent* HandSceneComponent;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Impact, meta = (AllowPrivateAccess = "true"))
    FTransform ClipTransform;
    
    UFUNCTION(BlueprintCallable)
    void GrabClip();
    
    // Combat Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    bool bAiming;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    bool bFireButtonPressed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    bool bFireShooting;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    EShootingType ShootingType;

    // Firing Configuration
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    float AutomaticFireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    float BurstFireRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    int32 Iterations;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    int32 CurrentIteration;

    // Weapon Properties
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Impact, meta = (AllowPrivateAccess = "true"))
    class AWeapon* EquippedWeapon;

    UPROPERTY(EditDefaultsOnly, Category = Combat)
    TSubclassOf<AWeapon> DefaultWeaponClass;

    UPROPERTY(VisibleAnywhere, Category = Combat)
    TMap<EAmmoType, int32> AmmoMap;

    // Effects & Animations
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    class UAnimMontage* FireMontage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
    class UAnimMontage* ReloadMontage;

    // Recoil system
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    bool bRecoilRecovering;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilPitch;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float MaxRecoilPitch;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilYawOffset;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilYawRandom;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilSmoothSpeed;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilRecoverTime;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilExtraDuration;
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Recoil, meta = (AllowPrivateAccess = "true"))
    float RecoilRecoverStrength;   
    
    // Components & References
    UPROPERTY()
    USkeletalMeshComponent* CharacterMeshComponent;

    // Timers
    FTimerHandle AutoFireTimer;
    FTimerHandle BurstFireTimer;
    FTimerHandle RecoilTimer;

    void InitializeAmmoMap();
    
    // Private Helper Functions
    void HandleAutoFire();
    void HandleBurstFire();
    void HandleSemiFire();
    void AutoFireReset();

    // Move to AdvancedMovementComp
    void AddControllerPitchInput(float Value);
    void AddControllerYawInput(float Value);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	class USoundCue* FireSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	class UParticleSystem* MuzzleFlash;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* HipFireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* ImpactParticles;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shooting", meta = (AllowPrivateAccess = "true"))
	UParticleSystem* BeamParticles;

	UFUNCTION()
	void SwitchShootingType();

	UFUNCTION()
	virtual bool TraceUnderCrosshairs(FHitResult& OutHitResult, FVector& OutHitLocation);
	
	UFUNCTION()
	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FVector& OutBeamLocation, FHitResult& WeaponTraceHit);
	
	UFUNCTION()
	void FireButtonPressed();

	UFUNCTION()
	void FireButtonReleased();
	
	UFUNCTION()
	void StartFireTimer();

	UFUNCTION()
	void AimingButtonPressed();
	
	UFUNCTION()
	void AimingButtonReleased();

	UPROPERTY()
	bool bShouldFire;

	UFUNCTION()
	void Aim();
	
	UFUNCTION()
	void StopAiming();

	UFUNCTION()
	bool GetAiming() const;
	
	class AWeapon* SpawnDefaultWeapon();
	void EquipWeapon(AWeapon* WeaponToEquip);

	UFUNCTION()
	void PlayFireSound();

	UFUNCTION()
	void SendBullet();

	UFUNCTION()
	void PlayGunFireMontage();

	UFUNCTION()
	void ReloadButtonPressed();
	
	UFUNCTION()
	void PlayReloadSound();
	
	UFUNCTION(BlueprintCallable)
	bool CarryingAmmo();
	
	UFUNCTION(BlueprintCallable)
	void ReleaseClip();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Impact, meta = (AllowPrivateAccess = "true"))
	bool CheckAutoFireRate;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Montages, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* RifleHipFireMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Sounds, meta = (AllowPrivateAccess = "true"))
	class USoundCue* EmptyMagazineSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	int32 Starting_556_Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	int32 Starting_762_Ammo;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Ammo, meta = (AllowPrivateAccess = "true"))
	int32 Starting_919_Ammo;

	// Decals
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	UDecalComponent* BeamDecal;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	class UMaterialInstance* ConcreteHoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	class UMaterialInstance* MetalHoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Decals")
	class UMaterialInstance* GlassHoleDecal;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> DestructionImpact;

	// FORCEINLINE
    FORCEINLINE AWeapon* GetEquippedWeapon() const { return EquippedWeapon; }
    FORCEINLINE ECombatState GetCombatState() const { return CombatState; }
};



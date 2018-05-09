// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactable.h"
#include "Pulse.h"
#include "WeaponBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AWeaponBase : public AInteractable
{
	GENERATED_BODY()
public:
		AWeaponBase();
		virtual void Tick(float DeltaTime) override;
		virtual void BeginPlay() override;
		virtual void OnInteract_Implementation(AActor* Caller) override;
		UFUNCTION(BlueprintCallable)
		void AddAmmo(float Amount);
		void SetAmmo(float Amount);
		void DealDamage(const FHitResult& Hit);
		void StartFire();
		void StopFire();
		void Reload();
		bool bSpawnFull = true;
		virtual void DoFire();
		FVector CalcSpread();
		void SpawnFireEffect();
		void SpawnImpactEffect(const FHitResult& Hit);
		void ChangeOwner(class APlayerCharacter* Player);
		
		void SpawnBullet();
		FVector AimDir();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly)
		FName MuzzleSocketName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category=Animation)
		UAnimSequence* ReloadSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite,Category = Animation)
		UAnimSequence* FireSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
		UAnimSequence* ZoomInSeq;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABullet> BulletClass;
	UPROPERTY(EditDefaultsOnly)
		FVector BulletVelocity;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly)
		float Price;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* WeaponImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float PriceMag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float PriceFull;
	bool bIsFiring;
	float CurrentAmmo;
	class APlayerCharacter* OwningPlayer;
	UPROPERTY(BlueprintReadOnly, Category = Spread)
		float Spread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spread)
		float MinSpread;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spread)
		float MaxSpread;
	void IncreaseSpread(float Value);
	inline bool HasFullAmmo() const {
		return CurrentAmmo == MaxAmmo ? true : false;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxAmmo() {
		return MaxAmmo;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetCurrentAmmo() {
		return CurrentAmmo;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetClipSize() {
		return ClipSize;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxClipSize() {
		return MaxClipSize;
	}
	UFUNCTION(BlueprintCallable)
		void AddMagazine();
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetBaseDamage() {
		return BaseDamage;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetFireRate() {
		return FireRate;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxRange() {
		return MaxRange;
	}
private:
	
	
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxAmmo = 0;
		float ClipSize;
	UPROPERTY(EditDefaultsOnly , Category = Stats)
		float MaxClipSize;
	UPROPERTY(EditDefaultsOnly, Category = Spread)
		float SpreadIncreaseSpeed;
	UPROPERTY(EditDefaultsOnly, Category = Spread)
		float SpreadDecreaseSpeed;
	UPROPERTY(EditDefaultsOnly , Category = Stats)
		float BaseDamage;
	UPROPERTY(EditDefaultsOnly,  Category = Stats)
		float FireRate;
	UPROPERTY(EditDefaultsOnly , Category = Stats)
		float MaxRange;
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* ShotEffect;
	UPROPERTY(EditDefaultsOnly)
		USoundWave* ShotSound;
	UPROPERTY(EditDefaultsOnly , Category = Stats)
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly , Category = Stats)
		USoundWave* ImpactSound;
	
	FTimerHandle BeamEffectTimerHandle;
	UPROPERTY(EditDefaultsOnly)
		float MaxBeamEffectTime;
	UParticleSystemComponent* BeamEffect;
	FCollisionQueryParams TraceParams;
	FTimerHandle FireRateHandle;
	UPROPERTY(EditDefaultsOnly)
	bool bCanReload;
	
public:
	UPROPERTY(EditDefaultsOnly, Category = Weapon)
		class UMaterial* Material;
	UPROPERTY(EditDefaultsOnly)
		EAmmoType AmmoType;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		EWeaponType WeaponType;
	UFUNCTION(BlueprintPure)
		USkeletalMeshComponent* GetMesh() const;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName WeaponName;
protected:
	UPROPERTY(EditDefaultsOnly, Category = FOV)
		float NormalFOV;
	UPROPERTY(EditDefaultsOnly, Category = FOV)
		float ZoomedFOV;
	UPROPERTY(BlueprintReadWrite)
		bool IsZooming;
	UPROPERTY(EditDefaultsOnly)
		bool bUsesProjectile;
};

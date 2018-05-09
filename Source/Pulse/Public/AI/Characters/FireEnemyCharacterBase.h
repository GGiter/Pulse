// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pulse.h"
#include "AI/Characters/BaseEnemyCharacter.h"
#include "FireEnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AFireEnemyCharacterBase : public ABaseEnemyCharacter
{
	GENERATED_BODY()
public:
	AFireEnemyCharacterBase();
	virtual void Attack() override;
	virtual void Tick(float DeltaTime) override;
	virtual void BeginPlay() override;
	UFUNCTION()
		virtual void EnableHunting(APawn* Pawn) override;

	//UPROPERTY(EditDefaultsOnly, meta = (EditCondition = "bUsesProjectile"))
//		TSubclassOf<class AProjectileBase> ProjectileClass;
	UPROPERTY(EditDefaultsOnly)
		FName MuzzleSocketName;
	uint32 bIsFiring : 1;
	float CurrentAmmo = 0;
	void PlayReloadAnim();
protected:

	//FPlayerInventory Inventory;
	UPROPERTY(EditDefaultsOnly,Category=Montage)
		UAnimMontage* FireMontage;
	UPROPERTY(EditDefaultsOnly,Category=Montage)
		UAnimMontage* ReloadMontage;
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<AWeaponBase> WeaponClass;
	UPROPERTY(EditDefaultsOnly)
		class AWeaponBase* CurrentWeapon;
	UPROPERTY(EditDefaultsOnly)
		FName WeaponSocketName;
	UPROPERTY(EditDefaultsOnly)
		uint32 bEquipNewWeapon : 1;
	virtual void DestroyWeapon() override;
	
	
	
	
};

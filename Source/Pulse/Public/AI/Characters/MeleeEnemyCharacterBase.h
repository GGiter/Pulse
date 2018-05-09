// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/BaseEnemyCharacter.h"
#include "Actors/Weapons/SideArmsBase.h"
#include "MeleeEnemyCharacterBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AMeleeEnemyCharacterBase : public ABaseEnemyCharacter
{
	GENERATED_BODY()

public:
	AMeleeEnemyCharacterBase();

	virtual void Tick(float DeltaTime) override;

	virtual void Attack() override;
	virtual void BeginPlay() override;

	UFUNCTION()
		virtual void EnableHunting(APawn *Pawn) override;


	UFUNCTION(BlueprintCallable, Category = Anim)
		void ChainCombo();
	UFUNCTION(BlueprintCallable, Category = Anim)
		void ComboReset();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ASideArmsBase> CloseCombatWeaponClass;
private:

	
	class ASideArmsBase* CloseCombatWeapon;
	bool bIsMeleeAttacking = false;
	bool bChainAttack = false;
	int32 Combo = 1;

	void HandleCombo();
	
	UPROPERTY(EditDefaultsOnly, Category = AnimMontages)
		UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = AnimMontages)
		UAnimMontage* SecondAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = AnimMontages)
		UAnimMontage* ThirdAttackMontage;
	UPROPERTY(EditAnywhere, Category = "Sockets")
		FName UpperSphereSocket = "UpperSphereSocket";
	UPROPERTY(EditAnywhere, Category = "Sockets")
		FName LowerSphereSocket = "LowerSphereSocket";
	UPROPERTY(EditAnywhere, Category = "Sockets")
		FName ForceAttackKey = "ForceAttack";
	UPROPERTY(EditAnywhere, Category = "Sockets")
		FName CloseCombatSocketName = "WeaponSocket";
protected:
	virtual void DestroyWeapon() override;
	
	
};

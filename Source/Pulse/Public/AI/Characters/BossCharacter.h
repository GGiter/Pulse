// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AI/Characters/BaseEnemyCharacter.h"
#include "BossCharacter.generated.h"

UCLASS()
class PULSE_API ABossCharacter : public ABaseEnemyCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = Stats)
		FORCEINLINE	float GetRemainingHealth() const { return Health; }

	virtual void Tick(float DeltaTime) override;

	virtual void Attack();
	UFUNCTION()
		void EnableHunting(APawn* Pawn);

	UFUNCTION(BlueprintCallable, Category = Anim)
		void ChainCombo();
	UFUNCTION(BlueprintCallable, Category = Anim)
		void ComboReset();
protected:
	UPROPERTY(EditAnywhere, Category = Combat)
		float YawModifier = 10.f;
private:

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
	UPROPERTY(EditDefaultsOnly)
		FName UpperHandSocket = "UpperHandSocket";
	UPROPERTY(EditDefaultsOnly)
		FName LowerHandSocket = "LowerHandSocket";
	
	
	
};

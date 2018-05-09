// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BaseEnemyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UBaseEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void PlayAttackAnimation();
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Proporties")
		FRotator RotatorDirection;
	UFUNCTION(BlueprintCallable)
		virtual void PlayGetHitAnimation();

	FORCEINLINE bool GetDamageRaycastStatus() { return bDamageRaycast; }

	UFUNCTION(BlueprintCallable)
		void SetDamageRaycastStatus(bool Status) { bDamageRaycast = Status; }

	FORCEINLINE void Die() { bIsDead = true; }

protected:
	

	UFUNCTION(BlueprintCallable, Category = "Proporties")
		void UpdatePawnSpeed();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
		UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Montage")
		UAnimMontage* GetHitMontage;

	bool bDamageRaycast : 1;

	UPROPERTY(BlueprintReadOnly, Category = "Proporties")
		bool bIsDead;
	
	
	
};

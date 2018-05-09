// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PulsePS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API APulsePS : public APlayerState
{
	GENERATED_BODY()
public:	
	UFUNCTION(BlueprintCallable)
		void LoadVariables();
	UFUNCTION(BlueprintCallable)
		void SaveVariables();
	void AddMoney(int32 Amount);
	UFUNCTION(BlueprintCallable)
		TArray<TSubclassOf<AWeaponBase>> GetStartingWeapons() const;
	UFUNCTION(BlueprintCallable, Category = Score)
		FORCEINLINE int32 GetMoney() const { return Money; }
	UFUNCTION(BlueprintCallable, Category = Score)
		void UpdateMoney(int32 Amount);
	virtual void BeginPlay() override;
	void SetStaringWeapons(TArray<TSubclassOf<AWeaponBase>> NewStaringWeapons);
private:
	UPROPERTY(Replicated)
		int32 Money;
	TArray<TSubclassOf<AWeaponBase>> StartingWeapons;

	
};

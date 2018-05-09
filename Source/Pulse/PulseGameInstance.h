// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Core.h"
#include "Engine/GameInstance.h"
#include "Engine/World.h"
#include "FindSessionsCallbackProxy.h"
#include "PulseGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class PULSE_API UPulseGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	UPulseGameInstance(const FObjectInitializer& FObjectInitializer);
	UPROPERTY(BlueprintReadWrite, Category = Player)
		TArray<FName> PlayerNames;
	UFUNCTION(BlueprintCallable)
		 void AddPlayerName(FName New);
	UFUNCTION(BlueprintCallable)
		TArray<FName> GetPlayerNames();
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxClipSize()
	{
		return MaxClipSize;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxDamage()
	{
		return MaxDamage;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxFireRate()
	{
		return MaxFireRate;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxRange()
	{
		return MaxRange;
	}
	UFUNCTION(BlueprintCallable,Server,WithValidation,Reliable)
		void ServerTravel(const FString& LevelName);

private:
	UPROPERTY(EditDefaultsOnly)
		float MaxClipSize;
	UPROPERTY(EditDefaultsOnly)
		float MaxDamage;
	UPROPERTY(EditDefaultsOnly)
		float MaxFireRate;
	UPROPERTY(EditDefaultsOnly)
		float MaxRange;

	
	
};

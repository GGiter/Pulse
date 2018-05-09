// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "LobbyPS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ALobbyPS : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable)
		void LoadVariables();
	UFUNCTION(BlueprintCallable)
		void SaveVariables();
	UFUNCTION(BlueprintCallable)
		void ChangeIsReady();
	UFUNCTION(Server, WithValidation, Reliable)
		void Server_ChangeIsReady();
	UFUNCTION(BlueprintCallable)
		void ChangePlayerName(FString NewName);
	UFUNCTION(Server, WithValidation, Reliable)
		void Server_ChangePlayerName(const FString& NewName);
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<class AWeaponBase>> WeaponsTemp;
		TArray<class AWeaponBase*>Weapons;
	UPROPERTY(BlueprintReadWrite)
		TArray<FBackpackItem>BackpackWeapons;
	UPROPERTY(BlueprintReadWrite)
		TArray<FBackpackItem> BackpackPlayer;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup)
		TArray<TSubclassOf<class AWeaponBase>> StartingWeapons;
	UPROPERTY(BlueprintReadWrite, Replicated)
		bool bIsReady = false;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup)
		TSubclassOf<class AGrenade> GrenadeClass;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup)
		TSubclassOf<class ASideArmsBase> CloseCombatWeaponClass;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup)
		bool bHasSetupWeapons;
};

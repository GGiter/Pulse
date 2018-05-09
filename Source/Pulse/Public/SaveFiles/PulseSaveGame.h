// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Pulse.h"
#include "PulseSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UPulseSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	
	UPulseSaveGame();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite,Category=Basic)
	FString PlayerName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
	uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = Player)
		TArray<FBackpackItem> Backpack;
	FString GetPlayerName() const
	{
		return PlayerName;
	}
	void SetPlayerName(const FString NewPlayerName)
	{
		PlayerName = NewPlayerName;
	}
};

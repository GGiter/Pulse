// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GameSetupSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UGameSetupSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UGameSetupSaveGame();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		FName LevelMapName;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		float WaveDelay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		float SpawnDelay;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		int32 MaxWaves;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
	TArray<struct FWaveInfo> WaveInfo;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	
	
};

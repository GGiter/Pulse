// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameState.h"
#include "LobbyGameState.generated.h"

/**
 * 
 */

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ELobbyState : uint8
{
	LS_Track	UMETA(DisplayName = "TrackMode"),
	LS_Horde 	UMETA(DisplayName = "HordeMode"),
	
};

UCLASS()
class PULSE_API ALobbyGameState : public AGameState
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable)
		void SaveGameSetup(FName NewLevelMapName, float NewWaveDelay, float NewSpawnDelay, int32 NewMaxWaves, TArray<int32> NewTotalNumberOfEnemies, TArray<TSubclassOf<class ABaseEnemyCharacter>> NewEnemyClass, TArray<int32> NewMaxAmount, TArray<float> NewProbability, bool bHasNewSetup);
	UPROPERTY(BlueprintReadWrite,Category=GameSetup,Replicated)
		FName LevelMapName;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		float WaveDelay;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		float SpawnDelay;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		int32 MaxWaves;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		TArray<int32> TotalNumberOfEnemies;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		TArray<TSubclassOf<class ABaseEnemyCharacter>> EnemyClass;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		TArray<int32> MaxAmount;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupHorde, Replicated)
		TArray<float> Probability;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup, Replicated)
		bool bHasSetupGame;
	UFUNCTION(BlueprintCallable)
		void SaveVariables(int32 PlayerId);
	UFUNCTION(BlueprintCallable)
		void SaveTrackVariables(int32 PlayerId);
	UPROPERTY(Replicated)
		TArray<struct FWaveInfo> WaveInfo;
	UPROPERTY(BlueprintReadWrite, Category = GameSetup, Replicated)
		ELobbyState LobbyState = ELobbyState::LS_Horde;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float StarsDensity= 0.003f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float StarsBrightness= 0.18f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		FVector2D NebulaScaleRange = { 0.001f,0.003f };
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float Steepness=0.5f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float Spacing=800.0f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float NumberOfPoints=6.0f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float EllipseWidth=30000.0f;
	UPROPERTY(BlueprintReadWrite, Category = GameSetupTrack, Replicated)
		float EllipseHeight = 30000.0f;
	
};

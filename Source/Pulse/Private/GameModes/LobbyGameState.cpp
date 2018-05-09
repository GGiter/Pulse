// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameState.h"
#include "Pulse.h"
#include "GameSetupSaveGame.h"
#include "SaveFiles/TrackSaveGame.h"


void ALobbyGameState::SaveGameSetup(FName NewLevelMapName, float NewWaveDelay, float NewSpawnDelay, int32 NewMaxWaves, TArray<int32> NewTotalNumberOfEnemies, TArray<TSubclassOf<class ABaseEnemyCharacter>> NewEnemyClass, TArray<int32> NewMaxAmount, TArray<float> NewProbability, bool bHasNewSetup)
{
	
		LevelMapName = NewLevelMapName;

		WaveDelay = NewWaveDelay;

		SpawnDelay = NewSpawnDelay;

		MaxWaves = NewMaxWaves;

		TotalNumberOfEnemies = NewTotalNumberOfEnemies;

		EnemyClass = NewEnemyClass;

		MaxAmount = NewMaxAmount;

		Probability = NewProbability;

		bHasSetupGame = bHasNewSetup;;
	
	
	
	
}



void ALobbyGameState::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameState, LevelMapName);
	DOREPLIFETIME(ALobbyGameState, WaveDelay);
	DOREPLIFETIME(ALobbyGameState, SpawnDelay);
	DOREPLIFETIME(ALobbyGameState, MaxWaves);
	DOREPLIFETIME(ALobbyGameState, TotalNumberOfEnemies);
	DOREPLIFETIME(ALobbyGameState, EnemyClass);
	DOREPLIFETIME(ALobbyGameState, MaxAmount);
	DOREPLIFETIME(ALobbyGameState, Probability);
	DOREPLIFETIME(ALobbyGameState, bHasSetupGame);
	DOREPLIFETIME(ALobbyGameState, WaveInfo);
	DOREPLIFETIME(ALobbyGameState, StarsDensity);
	DOREPLIFETIME(ALobbyGameState, StarsBrightness);
	DOREPLIFETIME(ALobbyGameState, NebulaScaleRange);
	DOREPLIFETIME(ALobbyGameState, Steepness);
	DOREPLIFETIME(ALobbyGameState, Spacing);
	DOREPLIFETIME(ALobbyGameState, NumberOfPoints);
	DOREPLIFETIME(ALobbyGameState, EllipseWidth);
	DOREPLIFETIME(ALobbyGameState, EllipseHeight);
	DOREPLIFETIME(ALobbyGameState, LobbyState);

}
void ALobbyGameState::SaveVariables(int32 PlayerId=0)
{
	UGameSetupSaveGame*SaveGameInstance = Cast<UGameSetupSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameSetupSaveGame::StaticClass()));
	SaveGameInstance->LevelMapName = LevelMapName;
	SaveGameInstance->WaveDelay = WaveDelay;
	SaveGameInstance->SpawnDelay = SpawnDelay;
	SaveGameInstance->MaxWaves = MaxWaves;
	for (int32 NumberOfEnemies : TotalNumberOfEnemies)
	{
		FWaveInfo NewWaveInfo;
		NewWaveInfo.TotalNumberOfEnemies = NumberOfEnemies;
		for (int32 i=0;i<NumberOfEnemies;++i)
		{
			FSpawnInfo NewSpawnInfo;
			NewSpawnInfo.EnemyClass = EnemyClass[i];
			NewSpawnInfo.MaxAmount = MaxAmount[i];
			NewSpawnInfo.Probability = Probability[i];
			NewWaveInfo.SpawnInfo.Add(NewSpawnInfo);
		}
		WaveInfo.Add(NewWaveInfo);
	}
	SaveGameInstance->WaveInfo = WaveInfo;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}
void ALobbyGameState::SaveTrackVariables(int32 PlayerId = 0)
{
	UTrackSaveGame*SaveGameInstance = Cast<UTrackSaveGame>(UGameplayStatics::CreateSaveGameObject(UTrackSaveGame::StaticClass()));
	SaveGameInstance->StarsDensity = StarsDensity;
	SaveGameInstance->StarsBrightness = StarsBrightness;
	SaveGameInstance->NebulaScaleRange = NebulaScaleRange;
	SaveGameInstance->Steepness = Steepness;
	SaveGameInstance->Spacing = Spacing;
	SaveGameInstance->NumberOfPoints = NumberOfPoints;
	SaveGameInstance->EllipseWidth = EllipseWidth;
	SaveGameInstance->EllipseHeight = EllipseHeight;
	SaveGameInstance->NumberOfPlayers = PlayerArray.Num();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}
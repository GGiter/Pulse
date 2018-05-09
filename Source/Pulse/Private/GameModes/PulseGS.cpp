// Fill out your copyright notice in the Description page of Project Settings.

#include "PulseGS.h"
#include "Pulse.h"
#include "Net/UnrealNetwork.h"
#include "GameSetupSaveGame.h"

void APulseGS::SetIsWaveActive(bool newActive)
{
	bIsWaveActive = newActive;
}
void APulseGS::AddEnemiesRemaining(int32 Amount)
{
	EnemiesRemaining += Amount;
}
void APulseGS::SetWaveDelay(float Delay)
{
	WaveDelay = Delay;
}
void APulseGS::SetMaxWaves(int32 Waves)
{
	MaxWaves = Waves;
}
void APulseGS::SetCurrentWave(int32 Wave)
{
	CurrentWave = Wave;
}
void APulseGS::GetWaves(int32& Max, int32& Current) const
{
	Max = MaxWaves;
	Current = CurrentWave;
}
void APulseGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APulseGS, EnemiesRemaining);
	DOREPLIFETIME(APulseGS, bIsWaveActive);
	DOREPLIFETIME(APulseGS, WaveDelay);
	DOREPLIFETIME(APulseGS, MaxWaves);
	DOREPLIFETIME(APulseGS, CurrentWave);
	DOREPLIFETIME(APulseGS, WaveInfo);
}
void APulseGS::LoadVariables(int32 PlayerId=0)
{
	UGameSetupSaveGame*LoadGameInstance = Cast<UGameSetupSaveGame>(UGameplayStatics::CreateSaveGameObject(UGameSetupSaveGame::StaticClass()));
	LoadGameInstance = Cast<UGameSetupSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, PlayerId));
	WaveDelay = LoadGameInstance->WaveDelay;
	MaxWaves = LoadGameInstance->MaxWaves;
	SpawnDelay = LoadGameInstance->SpawnDelay;
	WaveInfo = LoadGameInstance->WaveInfo;
}


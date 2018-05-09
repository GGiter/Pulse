// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelector.h"
#include "Pulse.h"
#include "Actors/LevelActor.h"



void ULevelSelector::NativeConstruct()
{
	Super::NativeConstruct();
	FActorSpawnParameters SpawnInfo;
	CurrentLevel = GetWorld()->SpawnActor<ALevelActor>(ConstructCurrentLevel, SpawnInfo);
}

void ULevelSelector::CallOnNextLevel()
{
	OnNextLevel.Broadcast();
}
void ULevelSelector::CallOnPreviousLevel()
{
	OnPreviousLevel.Broadcast();
}
void ULevelSelector::UpdateLevel(ALevelActor* NewLevel, int32 Index)
{
	CurrentLevel = NewLevel;
}
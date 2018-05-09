// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuPS.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "PulseSaveGame.h"


void AMainMenuPS::BeginPlay()
{
	

}
void AMainMenuPS::SetNewPlayerName(FString New)
{
	SetPlayerName(New);
}

void AMainMenuPS::SaveVariables()
{
	UPulseSaveGame*SaveGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	SaveGameInstance->PlayerName = GetPlayerName();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}
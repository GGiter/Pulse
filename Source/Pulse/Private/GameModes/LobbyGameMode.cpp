// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyGameMode.h"
#include "Pulse.h"
#include "LobbyPS.h"
#include "PulseSaveGame.h"

void ALobbyGameMode::PostLogin(APlayerController *NewPlayer) 
{
	Super::PostLogin(NewPlayer);
	UE_LOG(LogTemp, Warning, TEXT("PostLogin"));
	ALobbyPS *MyPlayerState = Cast<ALobbyPS>(NewPlayer->PlayerState);
	MyPlayerState->LoadVariables();
	
}
void ALobbyGameMode::SaveVariables()
{
	UPulseSaveGame*SaveGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	//SaveGameInstance->PlayerName = PlayerName;
	//SaveGameInstance->Backpack = BackpackPlayer;
	//UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}

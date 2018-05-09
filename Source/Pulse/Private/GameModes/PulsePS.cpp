// Fill out your copyright notice in the Description page of Project Settings.

#include "PulsePS.h"
#include "Pulse.h"
#include "Kismet/GameplayStatics.h"
#include "PulseSaveGame.h"
#include "PlayerCharacter.h"
#include "PulseGameMode.h"
#include "Net/UnrealNetwork.h"


void APulsePS::BeginPlay()
{
	Super::BeginPlay();
	
	
}
void APulsePS::LoadVariables()
{
	UPulseSaveGame*LoadGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	LoadGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, PlayerId));
	SetPlayerName((FString)LoadGameInstance->PlayerName);
	APlayerCharacter* Player = Cast<APlayerCharacter>(GetOwner());
	if (Player)
	{
		Player->Backpack = LoadGameInstance->Backpack;
	}
}

void APulsePS::SaveVariables()
{
	UPulseSaveGame*SaveGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	SaveGameInstance->PlayerName = GetPlayerName();
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}

void APulsePS::AddMoney(int32 Amount)
{
	Money += Amount;
}
void APulsePS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APulsePS, Money);

}
TArray<TSubclassOf<AWeaponBase>> APulsePS::GetStartingWeapons() const
{
	return StartingWeapons;
}
void APulsePS::SetStaringWeapons(TArray<TSubclassOf<AWeaponBase>> NewStaringWeapons)
{
	StartingWeapons = NewStaringWeapons;
}
void APulsePS::UpdateMoney(int32 Amount)
{
	Money += Amount;
}

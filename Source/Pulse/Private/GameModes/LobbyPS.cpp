// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPS.h"
#include "Pulse.h"
#include "Kismet/GameplayStatics.h"
#include "PulseSaveGame.h"
#include "Actors/Weapons/WeaponBase.h"


void ALobbyPS::BeginPlay()
{
	Super::BeginPlay();
	for (TSubclassOf<class AWeaponBase> Weapon : WeaponsTemp)
	{
		FActorSpawnParameters SpawnParams;
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		AWeaponBase* NewWeapon = GetWorld()->SpawnActor<AWeaponBase>(Weapon, Location, Rotation,SpawnParams);
		NewWeapon->SetActorHiddenInGame(true);
		Weapons.Add(NewWeapon);
	}
	for (AWeaponBase* Weapon : Weapons)
	{
		FBackpackItem NewItem;
		NewItem.Weapon = Weapon;
		NewItem.Material = Weapon->Material;
		BackpackWeapons.Add(NewItem);
	}
	bHasSetupWeapons = false;
}
void ALobbyPS::LoadVariables()
{
	UPulseSaveGame*LoadGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	LoadGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, PlayerId));
	SetPlayerName((FString)LoadGameInstance->GetPlayerName());
}
void ALobbyPS::SaveVariables()
{
	UPulseSaveGame*SaveGameInstance = Cast<UPulseSaveGame>(UGameplayStatics::CreateSaveGameObject(UPulseSaveGame::StaticClass()));
	SaveGameInstance->SetPlayerName(GetPlayerName());
	SaveGameInstance->Backpack = BackpackPlayer;
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveGameInstance->SaveSlotName, PlayerId);
}
void ALobbyPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyPS, bIsReady);
}
void ALobbyPS::Server_ChangeIsReady_Implementation()
{
	if (bIsReady)
		bIsReady = false;
	else
		bIsReady = true;
}
bool ALobbyPS::Server_ChangeIsReady_Validate()
{
	return true;
}
void ALobbyPS::ChangeIsReady()
{
	if (Role == ROLE_Authority)
	{
		if (bIsReady)
			bIsReady = false;
		else
			bIsReady = true;
	}
	else
	{
		Server_ChangeIsReady();
	}
}
void ALobbyPS::ChangePlayerName(FString NewName)
{
	if (Role == ROLE_Authority)
	{
		SetPlayerName(NewName);
	}
	else
	{
		Server_ChangePlayerName(NewName);
	}
}
void ALobbyPS::Server_ChangePlayerName_Implementation(const FString& NewName)
{
	SetPlayerName(NewName);
}
bool ALobbyPS::Server_ChangePlayerName_Validate(const FString& NewName)
{
	return true;
}
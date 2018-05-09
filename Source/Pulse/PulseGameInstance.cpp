// Fill out your copyright notice in the Description page of Project Settings.

#include "PulseGameInstance.h"
#include "GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/ActorComponent.h"
#include "Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Pulse.h"



UPulseGameInstance::UPulseGameInstance(const FObjectInitializer& ObjectInitializer)
	:Super(ObjectInitializer)
{
	
}
void UPulseGameInstance::AddPlayerName(FName New)
{
	PlayerNames.Add(New);
	
}
TArray<FName> UPulseGameInstance::GetPlayerNames()
{
	return PlayerNames;
}
void UPulseGameInstance::ServerTravel_Implementation(const FString& LevelName)
{
	GetWorld()->ServerTravel(LevelName);
}
bool UPulseGameInstance::ServerTravel_Validate(const FString& LevelName)
{
	return true;
}

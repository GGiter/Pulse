// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGameMode.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"

void AMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (MainMenuWidget)
	{
		MainMenuWidgetRef = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidget);
	
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Gamemode-Widget not found"));
	}
}

TSubclassOf<class UUserWidget> AMainMenuGameMode::GetMainMenuWidget()
{
	return MainMenuWidget;
}
void AMainMenuGameMode::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AMainMenuGameMode, MainMenuWidget);
	DOREPLIFETIME(AMainMenuGameMode, MainMenuWidgetRef);

}
// Fill out your copyright notice in the Description page of Project Settings.

#include "MainMenuGS.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"

void AMainMenuGS::BeginPlay()
{
	Super::BeginPlay();
	if (MainMenuWidget)
	{
		MainMenuWidgetRef = CreateWidget<UUserWidget>(GetWorld(), MainMenuWidget);
	}
}

TSubclassOf<class UUserWidget> AMainMenuGS::GetMainMenuWidget()
{
	return MainMenuWidget;
}
void AMainMenuGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	DOREPLIFETIME(AMainMenuGS, MainMenuWidget);
	DOREPLIFETIME(AMainMenuGS, MainMenuWidgetRef);

}




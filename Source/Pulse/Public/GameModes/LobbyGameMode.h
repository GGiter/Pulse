// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"


UCLASS()
class PULSE_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

		void PostLogin(APlayerController *NewPlayer) override;
	void SaveVariables();

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
private:
	
		TSubclassOf<class UUserWidget> MainMenuWidget;
	class UUserWidget* MainMenuWidgetRef;
	
	
	
};

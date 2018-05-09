// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AMainMenuGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	TSubclassOf<class UUserWidget> GetMainMenuWidget();
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,meta=(AllowPrivateAccess="true"),Replicated, Category = Widgets)
		TSubclassOf<class UUserWidget> MainMenuWidget;
	UPROPERTY(BlueprintReadOnly,meta=(AllowPrivateAccess="true"),Replicated, Category = Widgets)
		class UUserWidget* MainMenuWidgetRef;
	
	
	
};

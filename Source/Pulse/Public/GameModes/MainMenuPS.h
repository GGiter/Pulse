// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerState.h"
#include "MainMenuPS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AMainMenuPS : public APlayerState
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	UPROPERTY(BlueprintReadWrite, Category = Player)
		FName PlayerDisplayName;
	UFUNCTION(BlueprintCallable)
		void SetNewPlayerName(FString New);
	UFUNCTION(BlueprintCallable)
		void SaveVariables();
	
private:
	class UUserWidget* MainMenuWidgetRef;
	
	
	
};

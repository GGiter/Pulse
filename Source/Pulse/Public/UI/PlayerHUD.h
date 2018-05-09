// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()
public:
	void UpdateHUD();
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void BP_UpdateHUD();
	
	
};

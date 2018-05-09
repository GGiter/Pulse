// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TerminalUI.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UTerminalUI : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintReadWrite,Category=Terminal)
	class ATerminal* Terminal;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<class AWeaponBase> WeaponClass;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<class AVehicleBase> VehicleClass;
	UPROPERTY(BlueprintReadWrite)
		TSubclassOf<class APickupBase> PlayerObjectClass;
	UPROPERTY(BlueprintReadWrite)
	int WhatToShow;
	UPROPERTY(BlueprintReadWrite, Category = Prices)
	int HealthPriceX1 = 5;
	UPROPERTY(BlueprintReadWrite, Category = Prices)
	int ShieldPriceX1 = 3;
	
};

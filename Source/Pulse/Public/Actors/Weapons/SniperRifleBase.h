// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "SniperRifleBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ASniperRifleBase : public AWeaponBase
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> ZoomInHUDClass;
	
	
};

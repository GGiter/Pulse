// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/PickupBase.h"
#include "Pulse.h"
#include "AmmoPickup.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AAmmoPickup : public APickupBase
{
	GENERATED_BODY()
public:
	virtual void OnInteract_Implementation(AActor* Caller) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float Amount;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		EAmmoType AmmoType;
	
	
	
};

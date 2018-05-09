// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/PickupBase.h"
#include "HealthPickup.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AHealthPickup : public APickupBase
{
	GENERATED_BODY()
public:
	virtual void OnInteract_Implementation(AActor* Caller) override;
private:
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float Amount;
	
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "WeaponBase.h"
#include "LaserRifleBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ALaserRifleBase : public AWeaponBase
{
	GENERATED_BODY()
public:
	virtual void Tick(float DeltaTime) override;

protected: 
	UPROPERTY(EditDefaultsOnly)
		float RechargeAmount;
		float Charge;
		UPROPERTY(EditDefaultsOnly)
			UParticleSystemComponent* BeamEffect;
	
};

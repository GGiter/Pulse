// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "TrackPS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ATrackPS : public APlayerState
{
	GENERATED_BODY()
public:
	void SetRaceTime(float NewRaceTime);
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetRaceTime() const { return RaceTime; }
private:
	UPROPERTY(Replicated)
	float RaceTime;
	
	
};

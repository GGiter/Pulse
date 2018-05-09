// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerStart.h"
#include "PulsePlayerStart.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API APulsePlayerStart : public APlayerStart
{
	GENERATED_BODY()
public:
	/** Which team can start at this point */
	UPROPERTY(EditInstanceOnly, Category = Team)
		int32 SpawnTeam;

	/** Whether players can start at this point */
	UPROPERTY(EditInstanceOnly, Category = Team)
		uint32 bNotForPlayers : 1;
	
	
	
};

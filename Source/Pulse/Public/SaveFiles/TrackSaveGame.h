// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TrackSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UTrackSaveGame : public USaveGame
{
	GENERATED_BODY()
public:
	UTrackSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Basic)
		FString PlayerName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		FString SaveSlotName;
	UPROPERTY(VisibleAnywhere, Category = Basic)
		uint32 UserIndex;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float StarsDensity;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float StarsBrightness;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		FVector2D NebulaScaleRange;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float Steepness;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float Spacing;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float NumberOfPoints;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float EllipseWidth;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		float EllipseHeight;
	UPROPERTY(VisibleAnywhere, Category = Setup)
		int32 NumberOfPlayers;
};

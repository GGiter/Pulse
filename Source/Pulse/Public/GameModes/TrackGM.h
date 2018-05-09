// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Surroundings/TrackGenerator.h"
#include "Surroundings/SpaceSphere.h"
#include "Surroundings/FinishLine.h"
#include "Vehicles/VehicleBase.h"
#include "TrackGS.h"
#include "TrackGM.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ATrackGM : public AGameMode
{
	GENERATED_BODY()
	virtual void BeginPlay() override;
public:
	void SetupHUD();
	void SetupStartingPoint();
	void InitGameState() override;
	UFUNCTION(BlueprintCallable)
		void GenerateTrack();
	UFUNCTION(BlueprintCallable)
		void GenerateSpaceSphere();
	virtual void PostLogin(APlayerController* NewPlayer) override;
	void PlayerFinished(AVehicleBase* Player);
	void GameFinished();
	void GameStart();
	void SpawnFinishLine();
private:
	ATrackGenerator* TrackActor;
	ASpaceSphere* SpaceSphere;
	ATrackGS* TrackGS;
	AFinishLine* FinishLine;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATrackGenerator> TrackClass;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASpaceSphere> SpaceSphereClass;
	UPROPERTY(EditDefaultsOnly)
	float StarsDensity;
	UPROPERTY(EditDefaultsOnly)
	float StarsBrightness;
	UPROPERTY(EditDefaultsOnly)
	FVector2D NebulaScaleRange;
	UPROPERTY(EditDefaultsOnly)
	float Steepness;
	UPROPERTY(EditDefaultsOnly)
	float Spacing;
	UPROPERTY(EditDefaultsOnly)
	float NumberOfPoints;
	UPROPERTY(EditDefaultsOnly)
	float EllipseWidth;
	UPROPERTY(EditDefaultsOnly)
	float EllipseHeight;
	UPROPERTY(EditDefaultsOnly)
	bool  bIsNotInEditor = false;
	UPROPERTY(EditDefaultsOnly)
	float StartUpTime = 30.0f;
	FTimerHandle GameStartTimerHandle;
};

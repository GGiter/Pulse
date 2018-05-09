// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "Classes/Components/SplineComponent.h"
#include "Vehicles/VehicleBase.h"
#include "TrackGS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ATrackGS : public AGameState
{
	GENERATED_BODY()
public:
	void SetStartLocation(FVector Location);
	void SetEndLocation(FVector Location);
	void LoadVariables(int32 PlayerId);
	void AddFinisher(AVehicleBase* Player);
	void SetTrackSpline(USplineComponent* NewTrackSpline);
	void SetGameStarted(bool bNewGameStarted);
	void SetStartupTime(float NewStartupTime);
	void SetGameCountdownStarted(bool bNewGameCountdownStarted);
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetStartupTime() const { return StartupTime; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetHasGameCountdownStarted() const { return bGameCountdownStarted; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE bool GetHasGameStarted() const { return bGameStarted; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE int32 GetNumberOfPlayers() const { return NumberOfPlayers; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE USplineComponent* GetTrackSpline() const { return TrackSpline; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE TArray<AVehicleBase*> GetFinishers() const { return Finishers; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetStartLocation() const { return StartLocation; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetEndLocation() const { return EndLocation; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetStarsDensity() const { return StarsDensity; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetStarsBrightness() const { return StarsBrightness; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector2D GetNebulaScaleRange() const { return NebulaScaleRange; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSteepness() const { return Steepness; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetSpacing() const { return Spacing; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetNumberOfPoints() const { return NumberOfPoints; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetEllipseWidth() const { return EllipseWidth; }
	UFUNCTION(BlueprintPure)
	FORCEINLINE float GetEllipseHeight() const { return EllipseHeight; }
private:
	UPROPERTY(Replicated)
		FVector StartLocation;
	UPROPERTY(Replicated)
		FVector EndLocation;
	UPROPERTY(Replicated)
		float StarsDensity;
	UPROPERTY(Replicated)
		float StarsBrightness;
	UPROPERTY(Replicated)
		FVector2D NebulaScaleRange;
	UPROPERTY(Replicated)
		float Steepness;
	UPROPERTY(Replicated)
		float Spacing;
	UPROPERTY(Replicated)
		float NumberOfPoints;
	UPROPERTY(Replicated)
		float EllipseWidth;
	UPROPERTY(Replicated)
		float EllipseHeight;
	UPROPERTY(Replicated)
		TArray<AVehicleBase*> Finishers;
	UPROPERTY(Replicated)
		USplineComponent* TrackSpline;
	UPROPERTY(EditAnywhere,Replicated)
		int32 NumberOfPlayers=1;
	UPROPERTY(Replicated)
		bool bGameStarted = false;
	UPROPERTY(Replicated)
		bool bGameCountdownStarted = false;
	UPROPERTY(Replicated)
		float StartupTime=30.0f;
	
};

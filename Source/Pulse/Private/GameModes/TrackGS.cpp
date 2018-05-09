// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackGS.h"
#include "SaveFiles/TrackSaveGame.h"
#include "Net/UnrealNetwork.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

void ATrackGS::SetEndLocation(FVector Location)
{
	EndLocation = Location;
	UE_LOG(LogTemp, Warning, TEXT("Setting up GS"));
}
void ATrackGS::SetStartLocation(FVector Location)
{
	StartLocation = Location;
}
void ATrackGS::LoadVariables(int32 PlayerId)
{
	UTrackSaveGame*LoadGameInstance = Cast<UTrackSaveGame>(UGameplayStatics::CreateSaveGameObject(UTrackSaveGame::StaticClass()));
	LoadGameInstance = Cast<UTrackSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->SaveSlotName, PlayerId));
	StarsDensity = LoadGameInstance->StarsDensity;
	StarsBrightness = LoadGameInstance->StarsBrightness;
	NebulaScaleRange = LoadGameInstance->NebulaScaleRange;
	Steepness = LoadGameInstance->Steepness;
	Spacing = LoadGameInstance->Spacing;
	EllipseWidth = LoadGameInstance->EllipseWidth;
	NumberOfPoints = LoadGameInstance->NumberOfPoints;
	EllipseHeight = LoadGameInstance->EllipseHeight;
	NumberOfPlayers = LoadGameInstance->NumberOfPlayers;
}
void ATrackGS::AddFinisher(AVehicleBase* Player)
{
	Finishers.AddUnique(Player);
}
void ATrackGS::SetTrackSpline(USplineComponent * NewTrackSpline)
{
	TrackSpline = NewTrackSpline;
}

void ATrackGS::SetGameStarted(bool bNewGameStarted)
{
	bGameStarted = bNewGameStarted;
}

void ATrackGS::SetStartupTime(float NewStartupTime)
{
	StartupTime = NewStartupTime;
}

void ATrackGS::SetGameCountdownStarted(bool bNewGameCountdownStarted)
{
	bGameCountdownStarted = bNewGameCountdownStarted;
}

void ATrackGS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrackGS, StarsDensity);
	DOREPLIFETIME(ATrackGS, StarsBrightness);
	DOREPLIFETIME(ATrackGS, NebulaScaleRange);
	DOREPLIFETIME(ATrackGS, Steepness);
	DOREPLIFETIME(ATrackGS, Spacing);
	DOREPLIFETIME(ATrackGS, NumberOfPoints);
	DOREPLIFETIME(ATrackGS, EllipseWidth);
	DOREPLIFETIME(ATrackGS, EllipseHeight);
	DOREPLIFETIME(ATrackGS, StartLocation);
	DOREPLIFETIME(ATrackGS, EndLocation);
	DOREPLIFETIME(ATrackGS, Finishers);
	DOREPLIFETIME(ATrackGS, TrackSpline);
	DOREPLIFETIME(ATrackGS, NumberOfPlayers);
	DOREPLIFETIME(ATrackGS, bGameStarted);
	DOREPLIFETIME(ATrackGS, bGameCountdownStarted);
	DOREPLIFETIME(ATrackGS, StartupTime);
}

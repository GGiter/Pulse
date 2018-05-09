// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackPS.h"
#include "Net/UnrealNetwork.h"


void ATrackPS::SetRaceTime(float NewRaceTime)
{
	RaceTime = NewRaceTime;
}
void ATrackPS::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrackPS, RaceTime);

}
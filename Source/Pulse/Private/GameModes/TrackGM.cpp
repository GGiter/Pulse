// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackGM.h"
#include "Vehicles/VehicleController.h"


void ATrackGM::BeginPlay()
{
	
	
	FActorSpawnParameters SpawnParams;
	TrackActor=GetWorld()->SpawnActor<ATrackGenerator>(TrackClass,FVector(0.0f,0.0f,0.0f),FRotator(0.0f,0.0f,0.0f),SpawnParams);
	SpaceSphere = GetWorld()->SpawnActor<ASpaceSphere>(SpaceSphereClass, FVector(0.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	SpaceSphere->SetActorScale3D(FVector(EllipseWidth/100.0f, EllipseWidth/100.0f, EllipseWidth/100.0f));
	if (TrackActor)
		GenerateTrack();
	if (SpaceSphere)
		GenerateSpaceSphere();
	
}
void ATrackGM::SetupHUD()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicleBase::StaticClass(), Players);
	for (AActor* Player : Players)
	{
		AVehicleBase* CastedPlayer = Cast<AVehicleBase>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			CastedPlayer->SetFinishLocation(TrackGS->GetEndLocation());
			UE_LOG(LogTemp, Warning, TEXT("Location%s"), *TrackGS->GetEndLocation().ToString());
		}
	}
}
void ATrackGM::SetupStartingPoint()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicleBase::StaticClass(), Players);
	float off = 0.0f;
	for (AActor* Player : Players)
	{
		AVehicleBase* CastedPlayer = Cast<AVehicleBase>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			//CastedPlayer->SetActorLocation(TrackGS->GetStartLocation()+FVector(0.0f,0.0f,5550.0f),false,nullptr,ETeleportType::TeleportPhysics);
			UE_LOG(LogTemp, Warning, TEXT("SpawningActor At Location %s"), *TrackGS->GetStartLocation().ToString());
		}
		off += 1;
	}
	
}
void ATrackGM::GenerateTrack()
{
	TrackActor->SetSteepness(Steepness);
	TrackActor->SetNewSpacing(Spacing);
	TrackActor->SetNumberOfPoints(NumberOfPoints);
	TrackActor->SetEllipseWidth(EllipseWidth);
	TrackActor->SetEllpiseHeight(EllipseHeight);
	TrackActor->Generate();
}

void ATrackGM::GenerateSpaceSphere()
{
	SpaceSphere->SetStarsDensity(StarsDensity);
	SpaceSphere->SetStarsBrightness(StarsBrightness);
	SpaceSphere->SetNebulaScaleRange(NebulaScaleRange);
	SpaceSphere->Generate();
}
void ATrackGM::InitGameState()
{
	Super::InitGameState();
	TrackGS = Cast<ATrackGS>(GameState);
	if (TrackGS&&bIsNotInEditor)
	{
		TrackGS->LoadVariables(0);
		Steepness = TrackGS->GetSteepness();
		Spacing = TrackGS->GetSpacing();
		NumberOfPoints = TrackGS->GetNumberOfPoints();
		EllipseWidth = TrackGS->GetEllipseWidth();
		EllipseHeight = TrackGS->GetEllipseHeight();
		StarsDensity = TrackGS->GetStarsDensity();
		StarsBrightness = TrackGS->GetStarsBrightness();
		NebulaScaleRange = TrackGS->GetNebulaScaleRange();
	}
}
void ATrackGM::PostLogin(APlayerController * NewPlayer)
{
	Super::PostLogin(NewPlayer);
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Players);
	AVehicleBase* CastedPlayer = Cast<AVehicleBase>(NewPlayer->GetPawn());
	if (CastedPlayer)
	{
		//CastedPlayer->SetFinishLocation(TrackGS->GetEndLocation());
		//CastedPlayer->SetActorLocation(TrackGS->GetStartLocation()+FVector(0.0f,0.0f,50.0f) , false, nullptr, ETeleportType::TeleportPhysics);
	//Players[0]->SetActorLocation(TrackGS->GetStartLocation() + FVector(0.0f, 0.0f, 50.0f), false, nullptr, ETeleportType::TeleportPhysics);
	}
	if (TrackGS->PlayerArray.Num() == TrackGS->GetNumberOfPlayers())
	{
		UE_LOG(LogTemp, Warning, TEXT("All players connected.Setting up timer."));
		TrackGS = Cast<ATrackGS>(GameState);
		GetWorld()->GetTimerManager().SetTimer(GameStartTimerHandle, this, &ATrackGM::GameStart, StartUpTime, false);
		TrackGS->SetGameCountdownStarted(true);
		TrackGS->SetStartupTime(StartUpTime);
	}
}

void ATrackGM::PlayerFinished(AVehicleBase * Player)
{
	TrackGS->AddFinisher(Player);
	if (TrackGS->GetFinishers().Num() == TrackGS->PlayerArray.Num())
		GameFinished();
	else
		Player->GameEnded();
}

void ATrackGM::GameFinished()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicleBase::StaticClass(), Players);
	for (AActor* Player : Players)
	{
		AVehicleBase* CastedPlayer = Cast<AVehicleBase>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			CastedPlayer->GameEnded();
		}
		
	}
}

void ATrackGM::GameStart()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AVehicleBase::StaticClass(), Players);
	for (AActor* Player : Players)
	{
		AVehicleBase* CastedPlayer = Cast<AVehicleBase>(Player);
		if (CastedPlayer && CastedPlayer->IsPlayerControlled())
		{
			CastedPlayer->StartRaceCounter();
		}
		AVehicleController * CastedController = Cast<AVehicleController>(CastedPlayer->GetController());
		if (CastedController)
		{
			CastedController->SetMoveable(true);
		}
	}
	
	TrackGS->SetGameStarted(true);
}

void ATrackGM::SpawnFinishLine()
{
	FActorSpawnParameters SpawnParams;
	FinishLine = GetWorld()->SpawnActor<AFinishLine>(AFinishLine::StaticClass(), TrackGS->GetEndLocation(), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	
}


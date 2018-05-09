// Fill out your copyright notice in the Description page of Project Settings.

#include "FinishLine.h"
#include "GameModes/TrackGM.h"
#include "GameModes/TrackGS.h"
#include "Vehicles/VehicleBase.h"

// Sets default values
AFinishLine::AFinishLine()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->InitBoxExtent(BoxExtend);
	BoxCollision->OnComponentBeginOverlap.AddDynamic(this, &AFinishLine::LineCrossed);
}

void AFinishLine::LineCrossed(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	AVehicleBase* VehiclePlayer = Cast<AVehicleBase>(OtherActor);
	SetOwner(OtherActor);
	if (VehiclePlayer&&VehiclePlayer->IsPlayerControlled())
	{
		LineCrossedServer(VehiclePlayer);
		ATrackGS* TrackGS = Cast<ATrackGS>(UGameplayStatics::GetGameState(GetWorld()));
		if (TrackGS)
			TrackGS->AddFinisher(VehiclePlayer);
	}
	
}
void AFinishLine::LineCrossedServer_Implementation(AVehicleBase* Player)
{
	ATrackGM* TrackGM = Cast<ATrackGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TrackGM)
	{
		TrackGM->PlayerFinished(Player);
	}
}
bool AFinishLine::LineCrossedServer_Validate(AVehicleBase* Player)
{
	return true;
}
// Called when the game starts or when spawned
void AFinishLine::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AFinishLine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// Fill out your copyright notice in the Description page of Project Settings.

#include "PickupBase.h"
#include "Pulse.h"
#include "Player/PlayerCharacter.h"



APickupBase::APickupBase()
{

	PrimaryActorTick.bCanEverTick = true;
	
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasePickupMesh"));
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	RootComponent = CollisionSphere;
	CollisionSphere->SetCollisionProfileName("OverlapAllDynamic");
	PickupMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	PickupMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	bCanInteract = true;
	bReplicates = true;
	RespawnDelay = 5.f;
	RespawnDelayRange = 5.f;
	bIsActive = true;

}


void APickupBase::BeginPlay()
{

	Super::BeginPlay();
	if (bTouchInteracts)
	{
		CollisionSphere->OnComponentBeginOverlap.AddDynamic(this, &APickupBase::OnBeginOverlap);

	}


}

void APickupBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
void APickupBase::RespawnPickup()
{
	bIsActive = true;
	OnRespawned();
}
void APickupBase::OnPickedUp()
{
	if (PickupMesh)
	{
		PickupMesh->SetVisibility(false);
		PickupMesh->SetSimulatePhysics(false);
	}
}
void APickupBase::OnRespawned()
{
	if (PickupMesh)
	{
		PickupMesh->SetVisibility(true);
	
		
	}
}
void APickupBase::OnInteract_Implementation(AActor* Caller) {
	APlayerCharacter*Player = Cast<APlayerCharacter>(Caller);
	if (Player)
		Player->Item_PopUp(PickupName);
}

void APickupBase::OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (bCanInteract) {
		OnInteract(OtherActor);
	}
}
void APickupBase::SetCanInteract(bool NewInteract) {
	bCanInteract = NewInteract;
}

// Fill out your copyright notice in the Description page of Project Settings.

#include "Interactable.h"
#include "Pulse.h"



// Sets default values
AInteractable::AInteractable()
{

	PrimaryActorTick.bCanEverTick = true;
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	RootComponent = MeshComp;
	bReplicates = true;
	MeshComp->CustomDepthStencilValue = STENCIL_ENEMY_OUTLINE;
	
}


void AInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}


void AInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AInteractable::OnInteract_Implementation(AActor* Caller)
{
	UE_LOG(LogTemp, Warning, TEXT("Interact"));
}
void AInteractable::OnBeginFocus()
{

	MeshComp->SetRenderCustomDepth(true);
	
}
void AInteractable::OnEndFocus()
{
	MeshComp->SetRenderCustomDepth(false);
}

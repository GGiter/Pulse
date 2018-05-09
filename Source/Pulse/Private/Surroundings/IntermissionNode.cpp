// Fill out your copyright notice in the Description page of Project Settings.

#include "IntermissionNode.h"
#include "Pulse.h"
#include "AI/Controllers/BossAIController.h"

AIntermissionNode::AIntermissionNode()
{

	PrimaryActorTick.bCanEverTick = true;
	SceneComp = CreateDefaultSubobject<USceneComponent>(FName("SceneComp"));

	SetRootComponent(SceneComp);

	SMComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("SMComp"));

	SMComp->AttachTo(SceneComp);
	BoxComp = CreateDefaultSubobject<UBoxComponent>(FName("BoxComp"));
	BoxComp->AttachTo(SceneComp);

}


void AIntermissionNode::BeginPlay()
{
	Super::BeginPlay();
	if (SMComp)
	{
		SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SMComp->SetVisibility(false);
	}
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AIntermissionNode::OnBoxOverlap);

	BoxComp->OnComponentEndOverlap.AddDynamic(this, &AIntermissionNode::OnBoxEndOverlap);
}
void AIntermissionNode::OnBoxOverlap(class UPrimitiveComponent* HitComp, AActor* OverlapedActor, UPrimitiveComponent* OverlapedComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OverlapedActor)
	{
		ABossAIController* BossConRef = Cast<ABossAIController>(OverlapedActor->GetInstigatorController());

		if (BossConRef && BossConRef->GetCurrentIntermissionNode() == this && SMComp)
		{

			SMComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			SMComp->SetVisibility(true);


		}
	}
}
void AIntermissionNode::OnBoxEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		ABossAIController* BossConRef = Cast<ABossAIController>(OtherActor->GetInstigatorController());
		if (BossConRef && BossConRef->GetCurrentIntermissionNode() != this)
		{

			SMComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			SMComp->SetVisibility(false);
		}
	}
}



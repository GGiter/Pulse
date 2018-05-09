// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelActor.h"
#include "Pulse.h"



// Sets default values
ALevelActor::ALevelActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	LevelMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	LevelMesh->AttachToComponent(RootComponent,FAttachmentTransformRules::KeepWorldTransform);
}

// Called when the game starts or when spawned
void ALevelActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


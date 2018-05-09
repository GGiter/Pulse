// Fill out your copyright notice in the Description page of Project Settings.

#include "Grenade.h"
#include "Pulse.h"
#include "Player/PlayerCharacter.h"
#include "AI/Characters/BaseEnemyCharacter.h"



// Sets default values
AGrenade::AGrenade()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CollisionSphere;
	CollisionSphere->bGenerateOverlapEvents = true;
	CollisionSphere->bReturnMaterialOnMove = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>("StaticMesh");
	StaticMesh->SetupAttachment(RootComponent);
	
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	RadialForce = CreateDefaultSubobject<URadialForceComponent>("RadialForce");
	RadialForce->SetupAttachment(StaticMesh);
	ProjectileMovement->OnProjectileStop.AddDynamic(this, &AGrenade::OnStop);
	ProjectileMovement->InitialSpeed = 2000;
	ProjectileMovement->MaxSpeed = 2000;
	ProjectileMovement->ProjectileGravityScale = 2;
	
}

// Called when the game starts or when spawned
void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	StaticMesh->SetSimulatePhysics(true);
	StaticMesh->AddImpulseAtLocation(GetVelocity(), GetActorLocation());
	if(bExplodesAfterTime)
	GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &AGrenade::ExplodeAfterTime, ExplodeDelay, false);
	
}

// Called every frame
void AGrenade::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
}
void AGrenade::SpawnExplodeEffect()
{
	StaticMesh->SetHiddenInGame(true);
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ExplodeEffect, GetActorTransform(),true);
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), ExplodeSound,GetActorLocation(), GetActorRotation(), 1, 1, 0);
	SetLifeSpan(0.2f);
}
void AGrenade::OnStop(const FHitResult &ImpactResult)
{
	if (!bExplodesAfterTime)
	{
		if (bIsCausingRadiusDamage)
		{
			Explode(ImpactResult);
		}
		else
		{
			FPointDamageEvent DamageEvent;
			DamageEvent.Damage = MaxWeaponDamage;
			DamageEvent.HitInfo = ImpactResult;
			ImpactResult.GetActor()->TakeDamage(MaxWeaponDamage, DamageEvent, OwningCharacter->GetController(), this);
		}
		SpawnExplodeEffect();
	}
}
void AGrenade::Explode(const FHitResult &Result)
{
	CollisionSphere->SetWorldLocation(Result.ImpactPoint);
	CollisionSphere->SetSphereRadius(DamageRadius);
	TArray<AActor*> OverlapingActors;
	CollisionSphere->GetOverlappingActors(OverlapingActors);
	FPointDamageEvent DamageEvent;
	DamageEvent.Damage = MaxWeaponDamage;
	DamageEvent.HitInfo = Result;
	for (AActor* Actor : OverlapingActors)
	{
		Actor->TakeDamage(MaxWeaponDamage, DamageEvent, OwningCharacter->GetController(), this);
	}
	RadialForce->FireImpulse();
}
void AGrenade::ExplodeAfterTime()
{
	CollisionSphere->SetSphereRadius(DamageRadius);
	TArray<AActor*> OverlapingActors;
	CollisionSphere->GetOverlappingActors(OverlapingActors);
	FPointDamageEvent DamageEvent;
	DamageEvent.Damage = MaxWeaponDamage;
	UE_LOG(LogTemp, Warning, TEXT("Granade %d"), OverlapingActors.Num());
	
	for (AActor* Actor : OverlapingActors)
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(Actor);
		ABaseEnemyCharacter* Enemy = Cast<ABaseEnemyCharacter>(Actor);
		if((Player||Enemy)&&Actor&&OwningCharacter)
		Actor->TakeDamage(MaxWeaponDamage, DamageEvent, OwningCharacter->GetController(), this);
	}
	
	SpawnExplodeEffect();
	RadialForce->FireImpulse();
}
void AGrenade::SetOwningCharacter(ACharacter* NewChar)
{
	OwningCharacter = NewChar;
}
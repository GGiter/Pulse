// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Pulse.h"
#include "Actors/Weapons/WeaponBase.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(50.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);		// set up a notification for when this component hits something blocking

																						// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;
	TraceParams = FCollisionQueryParams(FName(TEXT("HitscanTrace")), true, this);
	Gravity = { 0, 0, -400 };
	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletMesh"));
	BulletMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	bReplicates = true;
	InitialLifeSpan = 4.0f;
	
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	if(AudioWave)
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), AudioWave, GetActorLocation());
	

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, GetActorLocation(), GetActorLocation()+(ProjectileMovement->Velocity*DeltaTime), ECC_Weapon, TraceParams);
	if (Hit.GetActor())
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit! %s"), *Hit.GetActor()->GetName());
		AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwner());
		if (Weapon)
		{
			Weapon->DealDamage(Hit);
			Weapon->SpawnImpactEffect(Hit);
		}
		FTransform HitTransform{ {0,0,0},Hit.Location,{1,1,1} };
		if(HitEffect)
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), HitEffect, HitTransform, true);
		Destroy();
	}
	else
	{
		SetActorLocation(GetActorLocation() + (ProjectileMovement->Velocity*DeltaTime));
		ProjectileMovement->Velocity = Gravity*DeltaTime + ProjectileMovement->Velocity;
		if (ProjectileMovement->Velocity.Size() > 9000)
		{
			ProjectileMovement->Velocity = (DeltaTime * 0.2f)*ProjectileMovement->Velocity + ProjectileMovement->Velocity;
		}
	}
	
}
void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 100.0f, GetActorLocation());

		
		if (Hit.GetActor()) {
			UE_LOG(LogTemp, Warning, TEXT("Hit! %s"), *Hit.GetActor()->GetName());
			AWeaponBase* Weapon = Cast<AWeaponBase>(GetOwner());
				if(Weapon)
				{
					Weapon->DealDamage(Hit);
					Weapon->SpawnImpactEffect(Hit);
				}
		}
		Destroy();
	}
}
void ABullet::InitVelocity(FVector ShootDirection)
{
	

	ProjectileMovement->Velocity = ShootDirection;
}

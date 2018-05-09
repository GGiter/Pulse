// Fill out your copyright notice in the Description page of Project Settings.

#include "SideArmsBase.h"
#include "Pulse.h"

#include "PlayerCharacter.h"


ASideArmsBase::ASideArmsBase()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh"));
	WeaponMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);

	PrimaryActorTick.bCanEverTick = true;
}


void ASideArmsBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsAttacking)
		DoDamage();
}
void ASideArmsBase::DoDamage()
{
	FVector UpperSphereSocketLocation = WeaponMesh->GetSocketLocation(UpperSphereSocket);
	FVector LowerSphereSocketLocation = WeaponMesh->GetSocketLocation(LowerSphereSocket);

	FVector CenterOfRaycast = ((UpperSphereSocketLocation - LowerSphereSocketLocation) / 2) + LowerSphereSocketLocation;
	TArray<FHitResult> HitResults;

	
	FCollisionShape CollisionShape;
	CollisionShape.ShapeType = ECollisionShape::Sphere;
	CollisionShape.SetSphere(AttackRaycastRadius);

	
	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);

	
	FCollisionQueryParams CollisionQueryParams;
	CollisionQueryParams.AddIgnoredActor(this);
	CollisionQueryParams.AddIgnoredActor(GetOwner());

	bool bHasHitActors = GetWorld()->SweepMultiByObjectType(HitResults, LowerSphereSocketLocation, UpperSphereSocketLocation, FQuat::Identity, ObjectQueryParams, CollisionShape, CollisionQueryParams);

	if (bHasHitActors)
	{
		for (auto It = HitResults.CreateIterator(); It; It++)
		{
			DoDamageServer(*It);
		}
	}
	
}
void ASideArmsBase::DoDamageServer_Implementation(const FHitResult& Hit)
{
	if (Hit.GetActor()) {
		UE_LOG(LogTemp, Warning, TEXT("Knife Damage"));
		float DealtDamage = BaseDamage;
		FVector ShotDir = GetActorLocation() - Hit.ImpactPoint;

		FPointDamageEvent DamageEvent;
		DamageEvent.Damage = DealtDamage;
		DamageEvent.HitInfo = Hit;
		DamageEvent.ShotDirection = ShotDir;
		DamageEvent.ShotDirection.Normalize();
		ACharacter* Char = Cast<ACharacter>(GetOwner());
		if(Char)
		Hit.GetActor()->TakeDamage(DealtDamage, DamageEvent, Char->GetController(), this);
	}
}
bool ASideArmsBase::DoDamageServer_Validate(const FHitResult& Hit)
{
	return true;
}
void ASideArmsBase::ChangeOnwer(APlayerCharacter* Player)
{
	OwningPlayer = Player;
}
void ASideArmsBase::SetIsAttacking(bool bNewIsAttacking)
{
	bIsAttacking = bNewIsAttacking;
}
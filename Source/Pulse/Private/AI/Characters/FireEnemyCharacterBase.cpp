// Fill out your copyright notice in the Description page of Project Settings.

#include "FireEnemyCharacterBase.h"
#include "Pulse.h"
#include "Actors/Interactable.h"
#include "Actors/Weapons/WeaponBase.h"
#include "Actors/Weapons/AssaultRifleBase.h"
#include "Actors/Weapons/LaserRifleBase.h"
#include "PlayerCharacter.h"
#include "MeleeEnemyCharacterBase.h"
#include "AI/Controllers/BaseAIController.h"
#include "AI/BaseEnemyAnimInstance.h"
#include "Perception/PawnSensingComponent.h"

AFireEnemyCharacterBase::AFireEnemyCharacterBase()
{

	



}
void AFireEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	FActorSpawnParameters SpawnParams;
	CurrentWeapon = GetWorld()->SpawnActor<AWeaponBase>(WeaponClass, GetActorLocation(), GetActorRotation(),SpawnParams);
	CurrentWeapon->OnInteract(this);
	CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, WeaponSocketName);
	CurrentWeapon->GetMesh()->SetOnlyOwnerSee(false);
	CurrentWeapon->GetMesh()->bCastDynamicShadow = false;
	CurrentWeapon->GetMesh()->CastShadow = false;
	CurrentWeapon->SetActorHiddenInGame(false);
	CurrentWeapon->SetOwner(this);
	
	
}
void AFireEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AFireEnemyCharacterBase::EnableHunting);

	}

}
void AFireEnemyCharacterBase::Attack()
{

	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
		GetMesh()->GetAnimInstance()->Montage_Play(FireMontage);
		UE_LOG(LogTemp, Warning, TEXT("FIRE!"));
		
	}



}
void AFireEnemyCharacterBase::PlayReloadAnim()
{
	GetMesh()->GetAnimInstance()->Montage_Play(ReloadMontage);
	
}
void AFireEnemyCharacterBase::EnableHunting(APawn* Pawn)
{

	ABaseAIController* AICon = Cast<ABaseAIController>(GetController());
	if (AICon)
	{
		AICon->SetNewTarget(Pawn);
		float MyDistanceFrom = (Pawn->GetActorLocation() - this->GetActorLocation()).Size();
		if (MyDistanceFrom <= MaxRange)
			AICon->SetShouldFire(true);
		else
			AICon->SetShouldFire(false);
	}
}
void AFireEnemyCharacterBase::DestroyWeapon()
{
	CurrentWeapon->Destroy();
}









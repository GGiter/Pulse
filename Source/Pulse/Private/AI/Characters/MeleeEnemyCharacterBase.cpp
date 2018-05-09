// Fill out your copyright notice in the Description page of Project Settings.

#include "MeleeEnemyCharacterBase.h"
#include "Pulse.h"

#include "PlayerCharacter.h"
#include "AI/Controllers/BaseAIController.h"
#include "AI/BaseEnemyAnimInstance.h"
#include "Perception/PawnSensingComponent.h"

AMeleeEnemyCharacterBase::AMeleeEnemyCharacterBase()
{




}
void AMeleeEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if (CloseCombatWeaponClass)
	{
		FActorSpawnParameters SpawnParams;
		CloseCombatWeapon = GetWorld()->SpawnActor<ASideArmsBase>(CloseCombatWeaponClass, GetActorLocation(), GetActorRotation(), SpawnParams);
		CloseCombatWeapon->SetActorHiddenInGame(false);
		CloseCombatWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, CloseCombatSocketName);
		CloseCombatWeapon->SetOwner(this);
	}
}
void AMeleeEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &AMeleeEnemyCharacterBase::EnableHunting);
	}
}
void AMeleeEnemyCharacterBase::Attack()
{
	
	if (bIsMeleeAttacking)
	{
		bChainAttack = true;
		
	}
	else
	{
		bIsMeleeAttacking = true;
		CloseCombatWeapon->SetIsAttacking(true);
		HandleCombo();
	}
}

void AMeleeEnemyCharacterBase::EnableHunting(APawn* Pawn)
{

	ABaseAIController* AICon = Cast<ABaseAIController>(GetController());
	if (AICon) AICon->SetNewTarget(Pawn);
}

void AMeleeEnemyCharacterBase::ChainCombo()
{
	if (bChainAttack)
	{
		bChainAttack = false;
		HandleCombo();
	}
}

void AMeleeEnemyCharacterBase::ComboReset()
{
	bIsMeleeAttacking = false;
	bChainAttack = false;
	Combo = 1;
	
	CloseCombatWeapon->SetIsAttacking(false);
}

void AMeleeEnemyCharacterBase::HandleCombo()
{
	switch (Combo)
	{
	case 1:
		Combo++;
		GetMesh()->GetAnimInstance()->Montage_Play(AttackMontage);
		break;
	case 2:
		Combo++;
		GetMesh()->GetAnimInstance()->Montage_Play(SecondAttackMontage);
		break;
	case 3:
		GetMesh()->GetAnimInstance()->Montage_Play(ThirdAttackMontage);
		break;
	}
}
void AMeleeEnemyCharacterBase::DestroyWeapon()
{
	CloseCombatWeapon->Destroy();
}

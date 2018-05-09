// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemyAnimInstance.h"
#include "Pulse.h"


void UBaseEnemyAnimInstance::UpdatePawnSpeed()
{
	APawn* Pawn = TryGetPawnOwner();
	if (Pawn)
	{
	
		RotatorDirection = Pawn->GetActorRotation();
	}
}
void UBaseEnemyAnimInstance::PlayAttackAnimation()
{
	if (AttackMontage && !Montage_IsPlaying(AttackMontage))
	{
		Montage_Play(AttackMontage);
	}
}
void UBaseEnemyAnimInstance::PlayGetHitAnimation()
{
	if (GetHitMontage && !Montage_IsPlaying(GetHitMontage))
	{

		Montage_Play(GetHitMontage);

	}
}



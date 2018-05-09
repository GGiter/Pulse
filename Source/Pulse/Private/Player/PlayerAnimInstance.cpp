// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAnimInstance.h"
#include "Pulse.h"
#include "PlayerCharacter.h"

void UPlayerAnimInstance::UpdateAnimationProporties()
{
	APawn* Pawn = TryGetPawnOwner();
	
		APlayerCharacter *Player = Cast<APlayerCharacter>(GetOwningActor());
		if (Player)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hello"));
			bIsFalling = Pawn->GetMovementComponent()->IsFalling();
			bIsSprinting = Player->GetIsRunning();
			bHasGun = Player->HasGun();
			MovementSpeed = Pawn->GetVelocity().Size();
			RotatorDirection = Pawn->GetActorRotation();
			bCrouching = Pawn->GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch;
			
		}
	
}
void UPlayerAnimInstance::Fire()
{
	PlaySlotAnimationAsDynamicMontage(FireSeq, TEXT("Normal"), 0.1f, 0.1f, 1.0f);
}
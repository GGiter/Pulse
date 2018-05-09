// Fill out your copyright notice in the Description page of Project Settings.

#include "BossCharacter.h"
#include "Pulse.h"
#include "AI/Controllers/BaseAIController.h"
#include "AI/Controllers/BossAIController.h"
#include "AI/BaseEnemyAnimInstance.h"

void ABossCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsMeleeAttacking)
	{

		//Fix the rotation of the boss in order to hit the target if in range
		ABaseAIController* Controller = Cast<ABaseAIController>(GetController());
		if (Controller)
		{
			AActor* CurrentTarget = Cast<ACharacter>(Controller->GetCurrentTarget());

			if (CurrentTarget)
			{
				FRotator NewRotation = (CurrentTarget->GetActorLocation() - GetActorLocation()).Rotation();
				NewRotation.Pitch = 0;
				NewRotation.Yaw -= YawModifier;
				SetActorRotation(NewRotation);
			}
		}

		FVector UpperSocket = GetMesh()->GetSocketLocation(UpperHandSocket);
		FVector LowerSocket = GetMesh()->GetSocketLocation(LowerHandSocket);

		FCollisionShape CollisionShape;
		CollisionShape.ShapeType = ECollisionShape::Line;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Pawn);
		ObjectQueryParams.AddObjectTypesToQuery(ECollisionChannel::ECC_Destructible);

		FCollisionQueryParams CollisionQueryParams;
		CollisionQueryParams.AddIgnoredActor(this);

		FQuat DefaultQuaternion;

		DrawDebugLine(GetWorld(), UpperSocket, LowerSocket, FColor::Red, false);

		TArray<FHitResult> HitActors;

		bool CollisionResult = GetWorld()->SweepMultiByObjectType(HitActors, UpperSocket, LowerSocket, DefaultQuaternion, ObjectQueryParams, CollisionShape, CollisionQueryParams);

		if (CollisionResult)
		{
			FDamageEvent dmgevent;
			for (auto It = HitActors.CreateConstIterator(); It; It++)
			{
				
				(*It).GetActor()->TakeDamage(BaseDamageAmount, dmgevent, GetController(), this);
			}
		}

	}
	if (PawnSensingComp)
	{
		PawnSensingComp->OnSeePawn.AddDynamic(this, &ABossCharacter::EnableHunting);

	}
}

void ABossCharacter::Attack()
{
	if (bIsMeleeAttacking)
	{
		bChainAttack = true;
	}
	else
	{
		bIsMeleeAttacking = true;
		HandleCombo();
	}
}
void ABossCharacter::EnableHunting(APawn* Pawn)
{
	ABossAIController* AICon = Cast<ABossAIController>(GetController());
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
void ABossCharacter::ChainCombo()
{
	if (bChainAttack)
	{
		bChainAttack = false;
		HandleCombo();
	}
}

void ABossCharacter::ComboReset()
{
	bIsMeleeAttacking = false;
	bChainAttack = false;
	Combo = 1;
}

void ABossCharacter::HandleCombo()
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


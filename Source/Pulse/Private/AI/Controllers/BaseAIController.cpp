// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAIController.h"
#include "Pulse.h"

#include "AI/Characters/BaseEnemyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Surroundings/AIPatrolNode.h"

ABaseAIController::ABaseAIController()
{
	BehaviorComp = CreateDefaultSubobject<UBehaviorTreeComponent>(FName("BehaviorComp"));
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(FName("BlackboardComp"));

}

void ABaseAIController::Possess(APawn * Pawn)
{
	Super::Possess(Pawn);
	ABaseEnemyCharacter* AICharacter = Cast<ABaseEnemyCharacter>(Pawn);
	if (AICharacter)
	{
		if (AICharacter->BehaviorTree->BlackboardAsset)
		{
			BlackboardComp->InitializeBlackboard(*(AICharacter->BehaviorTree->BlackboardAsset));
			BehaviorComp->StartTree(*AICharacter->BehaviorTree);
		}
	}
}

void ABaseAIController::UnPossess()
{
	Super::UnPossess();

	BehaviorComp->StopTree();
}

/*Sets a new target*/
void ABaseAIController::SetNewTarget(AActor* ActorToTarget)
{
	if (BlackboardComp)
		BlackboardComp->SetValueAsObject(TargetKey, ActorToTarget);
}
/*Return the current target of the AI*/
AActor* ABaseAIController::GetCurrentTarget()
{
	AActor* CurrentTarget = Cast<AActor>(BlackboardComp->GetValueAsObject(TargetKey));
	return CurrentTarget;
}
/*Sets a new patrol node for the AI*/
void ABaseAIController::SetNewPatrolNode(AActor* Node)
{

	AAIPatrolNode *OldPatrolNode = Cast<AAIPatrolNode>(BlackboardComp->GetValueAsObject(PatrolNodeKey));

	if (OldPatrolNode) OldPatrolNode->SetOccupyStatus(false);
	AAIPatrolNode* NewPatrolNode = Cast<AAIPatrolNode>(Node);

	if (NewPatrolNode)
	{

		NewPatrolNode->SetOccupyStatus(true);
		BlackboardComp->SetValueAsObject(PatrolNodeKey, Node);
	}
}
void ABaseAIController::SetShouldFire(bool New)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(ShouldFireKey, New);
		BlackboardComp->SetValueAsBool(CheckerKey, true);
	}
}









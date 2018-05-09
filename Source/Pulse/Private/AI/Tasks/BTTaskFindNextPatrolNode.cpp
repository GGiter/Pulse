// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTaskFindNextPatrolNode.h"
#include "Pulse.h"
#include "Surroundings/AIPatrolNode.h"
#include "AI/Characters/MeleeEnemyCharacterBase.h"
#include "AI/Controllers/BaseAIController.h"

#define MAX_ATTEMPS 500

EBTNodeResult::Type UBTTaskFindNextPatrolNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ABaseAIController* AICon = Cast<ABaseAIController>(OwnerComp.GetAIOwner());

	if (AICon)
	{
		TArray<AActor*> AIPatrolNodes;

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIPatrolNode::StaticClass(), AIPatrolNodes);

		bool bFoundNextNode = false;
		AAIPatrolNode* NextNodeToOccupy = nullptr;
		int32 CurrentAttempts = 0;

		/*Find the next avaible node*/
		while (!bFoundNextNode && CurrentAttempts < MAX_ATTEMPS)
		{
			CurrentAttempts++;

			NextNodeToOccupy = Cast<AAIPatrolNode>(AIPatrolNodes[FMath::RandRange(0, AIPatrolNodes.Num() - 1)]);
			if (NextNodeToOccupy && !NextNodeToOccupy->IsOccupied())
			{
				AICon->SetNewPatrolNode(NextNodeToOccupy);

				return EBTNodeResult::Succeeded;
			}
		}
	}


	return EBTNodeResult::Failed;
}

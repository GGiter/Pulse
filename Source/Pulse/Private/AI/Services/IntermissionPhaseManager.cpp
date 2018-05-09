// Fill out your copyright notice in the Description page of Project Settings.

#include "IntermissionPhaseManager.h"
#include "Pulse.h"
#include "AI/Characters/BossCharacter.h"
#include "AI/Controllers/BossAIController.h"

void UIntermissionPhaseManager::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaTime)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaTime);
	ABossAIController* Con = Cast<ABossAIController>(OwnerComp.GetAIOwner());
	if (Con)
	{
		Con->PerformIntermissionCheck();
	}
}





// Fill out your copyright notice in the Description page of Project Settings.

#include "BossAIController.h"
#include "Pulse.h"
#include "AI/Characters/BossCharacter.h"
#include "Surroundings/IntermissionNode.h"


void ABossAIController::Possess(APawn* InPawn)
{
	Super::Possess(InPawn);

	SetIntermissionPhaseStatus(false);

	int32 TotalIntermissions = HealthPercentForAnIntermissionPhase.Num();

	ActivatedIntermissionPhases.Init(false, TotalIntermissions);

	ABossCharacter* Boss = Cast <ABossCharacter>(InPawn);
	if (Boss)
	{
		BossMaxHealth = Boss->GetRemainingHealth();
		SetNewTarget(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		TArray<AActor*> OutActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AIntermissionNode::StaticClass(), OutActors);
		for (auto It = OutActors.CreateIterator(); It; It++)
		{
			if ((*It) && (*It)->IsA<AIntermissionNode>())
			{
				IntermissionNodesArray.Add(Cast<AIntermissionNode>(*It));
			}
		}
	}
}
void ABossAIController::PerformIntermissionCheck()
{
	ABossCharacter* Boss = Cast<ABossCharacter>(GetPawn());
	if (Boss)
	{
		
		if (BlackboardComp&&BlackboardComp->GetValueAsBool(IntermissionPhaseKey))
		{
			float CurrentDurationOfIntermission = IntermissionPhaseStartTime - GetWorld()->GetRealTimeSeconds();

			CurrentDurationOfIntermission = FMath::Abs<float>(CurrentDurationOfIntermission);

			if (CurrentDurationOfIntermission >= IntermissionDuration)
			{
				SetIntermissionPhaseStatus(false);
				bIsInIntermission = false;

				return;
			}
		}
		int32 NextIntermissionPhase = ActivatedIntermissionPhases.Find(false);
		if (NextIntermissionPhase != INDEX_NONE)
		{
			float HealthPercent = HealthPercentForAnIntermissionPhase[NextIntermissionPhase];
			
			if (((Boss->GetRemainingHealth() / BossMaxHealth) * 100) <= HealthPercent)
			{
				ActivatedIntermissionPhases[NextIntermissionPhase] = true;

				SetIntermissionPhaseStatus(true);
				bIsInIntermission = true;
			}
		}
	}
}
AIntermissionNode* ABossAIController::GetCurrentIntermissionNode()
{
	if (BlackboardComp)
	{
		AIntermissionNode* CurrentNode = Cast<AIntermissionNode>(BlackboardComp->GetValueAsObject(IntermissionNodeKey));
		return CurrentNode;
	}
	return nullptr;
}
void ABossAIController::SetIntermissionPhaseStatus(bool Status)
{
	if (BlackboardComp && Status)
	{
		BlackboardComp->SetValueAsBool(IntermissionPhaseKey, Status);

		FindTheFarthestIntermissionNode();

		IntermissionPhaseStartTime = GetWorld()->GetRealTimeSeconds();
	}
	else if (BlackboardComp && !Status)
	{
		//Updating the values of our blackboard to match the current state of the fight
		BlackboardComp->SetValueAsBool(IntermissionPhaseKey, Status);

		BlackboardComp->SetValueAsObject(IntermissionNodeKey, nullptr);
	}
}
void ABossAIController::FindTheFarthestIntermissionNode()
{
	
	if (IntermissionNodesArray.IsValidIndex(0) && GetPawn())
	{
		
		float MaxDistance = (IntermissionNodesArray[0]->GetActorLocation() - GetPawn()->GetActorLocation()).Size();
		AIntermissionNode* FarthestNode = IntermissionNodesArray[0];

		
		for (int32 Index = 1; Index < IntermissionNodesArray.Num(); Index++)
		{
			if (IntermissionNodesArray[Index] && MaxDistance<(IntermissionNodesArray[Index]->GetActorLocation() - GetPawn()->GetActorLocation()).Size())
			{
				MaxDistance = (IntermissionNodesArray[Index]->GetActorLocation() - GetPawn()->GetActorLocation()).Size();

				FarthestNode = IntermissionNodesArray[Index];
			}
		}

		

		if (BlackboardComp) BlackboardComp->SetValueAsObject(IntermissionNodeKey, FarthestNode);

	}
}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
public:
	ABaseAIController();
	virtual void Possess(APawn * Pawn) override;
	virtual void UnPossess() override;
	/*Set new target of AI*/
	void SetNewTarget(AActor* ActorToTarget);
	/*Returns current target of AI*/
	AActor* GetCurrentTarget();
	/*Set new patrol node of AI*/
	void SetNewPatrolNode(AActor* Node);
	/*Set distance between actor and AI*/
	void SetShouldFire(bool New);
	/*Return the blackboard component for this controller*/
	FORCEINLINE UBlackboardComponent* GetBlackboardComp() { return BlackboardComp; }
	/*Return the behavior tree component for this controller*/
	FORCEINLINE UBehaviorTreeComponent* GetBehaviorTreeComp() { return BehaviorComp; }

protected:
	UBehaviorTreeComponent* BehaviorComp;
	UBlackboardComponent* BlackboardComp;
	/*Blackboard TargetKey option*/
	const FName TargetKey = "TargetKey";
	/*Blackboard AIPatrolNode key option*/
	const FName PatrolNodeKey = "PatrolNodeKey";
	const FName ShouldFireKey = "ShouldFireKey";
	const FName CheckerKey = "CheckerKey";
	
	
	
};

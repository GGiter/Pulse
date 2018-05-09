// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "IntermissionPhaseManager.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UIntermissionPhaseManager : public UBTService
{
	GENERATED_BODY()
	
public:
	virtual void TickNode(UBehaviorTreeComponent& OnwerComp, uint8* NodeMemory, float DeltaTime) override;

	
	
};

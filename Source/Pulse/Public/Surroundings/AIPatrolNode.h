// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AIPatrolNode.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AAIPatrolNode : public ATargetPoint
{
	GENERATED_BODY()
	

public:
	AAIPatrolNode();
	/*Set new occupy status on the current node*/
	void SetOccupyStatus(bool NewStatus);
	FORCEINLINE uint32 IsOccupied() { return bIsOccupied; }
private:
	uint32 bIsOccupied : 1;

	
	
};

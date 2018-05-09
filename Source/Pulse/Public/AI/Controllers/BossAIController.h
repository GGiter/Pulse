// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AI/Controllers/BaseAIController.h"
#include "BossAIController.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ABossAIController : public ABaseAIController
{
	GENERATED_BODY()
private:
	/*True activates the intermission false deactivates it*/
	void SetIntermissionPhaseStatus(bool Status);
	/*Holds a reference to all the intermission nodes on the current level*/
	TArray<class AIntermissionNode*> IntermissionNodesArray;
	/*Finds the farthest intermission node based on the boss's position and updates its blackboard value*/
	void FindTheFarthestIntermissionNode();
	/*Determine the intermission phase that needs to be activated when the boss
	reaches a certain health percentage*/
	TArray<bool> ActivatedIntermissionPhases;

	float IntermissionPhaseStartTime;

	float BossMaxHealth;

	bool bIsInIntermission;

	const FName IntermissionPhaseKey = "IsInIntermissionPhase";
	const FName IntermissionNodeKey = "IntermissionNode";
	const FName IntermissionDurationKey = "IntermissionPhaseDuration";

protected:
	UPROPERTY(EditAnywhere, Category = Intermission)
		TArray<float> HealthPercentForAnIntermissionPhase;

	UPROPERTY(EditAnywhere, Category = Intermission)
		float IntermissionDuration;
public:
	virtual void Possess(APawn* Pawn) override;
	/*Determines if the boss should go to intermission phase*/
	void PerformIntermissionCheck();
	/*Returns the current intermission node - always check for nullptr after using this*/
	AIntermissionNode* GetCurrentIntermissionNode();


	
};

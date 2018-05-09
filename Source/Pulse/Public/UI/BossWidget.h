// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/Characters/BossCharacter.h"
#include "Blueprint/UserWidget.h"
#include "BossWidget.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UBossWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE ABossCharacter* GetBossRef() const
	{
		return BossRef;
	}
	void SetBossRef(ABossCharacter* NewBossRef);
private:
	ABossCharacter * BossRef;
	
	
};

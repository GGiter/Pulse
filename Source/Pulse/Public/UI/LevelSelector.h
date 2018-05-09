// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LevelSelector.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNextLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPreviousLevel);


UCLASS()
class PULSE_API ULevelSelector : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	UPROPERTY(BlueprintAssignable, Category = Delegates)
		FOnNextLevel OnNextLevel;
	UPROPERTY(BlueprintAssignable, Category = Delegates)
		FOnPreviousLevel OnPreviousLevel;
	UFUNCTION(BlueprintCallable, Category = Delegates)
		void CallOnNextLevel();
	UFUNCTION(BlueprintCallable, Category = Delegates)
		void CallOnPreviousLevel();
	UPROPERTY(BlueprintReadOnly, Category = Levels)
		class ALevelActor* CurrentLevel;
	UFUNCTION(BlueprintCallable, Category = Levels)
		void UpdateLevel(class ALevelActor* NewLevel, int32 Index);
	UPROPERTY(EditDefaultsOnly, Category = Levels)
		TSubclassOf<ALevelActor> ConstructCurrentLevel;
	
	
};

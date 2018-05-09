// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Pulse.h"
#include "GameFramework/Actor.h"
#include "LevelSelectorActor.generated.h"

UCLASS()
class PULSE_API ALevelSelectorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ALevelSelectorActor();
	UFUNCTION(BlueprintCallable)
		void PreviousLevel();
	UFUNCTION(BlueprintCallable)
		void NextLevel();
	UFUNCTION(BlueprintCallable)
		void TogglePanel();
	UPROPERTY(BlueprintReadOnly)
		USceneComponent* LevelsRoot;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Widget)
		TSubclassOf<class UUserWidget> Widget;
	UPROPERTY(EditDefaultsOnly, Category = Levels)
		TArray<TSubclassOf<class ALevelActor>> LevelsToChoose;
	UPROPERTY(BlueprintReadOnly)
		UUserWidget* MyWidget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsPanelVisible = false;
	UFUNCTION(BlueprintCallable)
		void SetupWidget(class UUserWidget* NewWidget);
	
private:
	void HideAllLevels();
	void ActivateUI();
	void ShowCurrentLevel();
	void SpawnAllLevels();
	void BindEvents();
	void BindOnNearLevel();
	


	int32 CurrentLevelIndex = 0;
	int32 MaxLevelIndex = 0;
	TArray<class ALevelActor*> Levels;
	TArray<class ALevelActor*> LevelsTemp;
	
protected:
	virtual void BeginPlay() override;
};

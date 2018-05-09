// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelSelectorActor.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"
#include "LevelActor.h"
#include "UI/LevelSelector.h"


ALevelSelectorActor::ALevelSelectorActor()
{
 	
	PrimaryActorTick.bCanEverTick = true;
	LevelsRoot = CreateDefaultSubobject<USceneComponent>("LevelsRoot");
	LevelsRoot->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
}


void ALevelSelectorActor::BeginPlay()
{
	
	if (Widget)
	{
		MyWidget = CreateWidget<UUserWidget>(GetWorld(), Widget);
	}
	CurrentLevelIndex = 0;
	SpawnAllLevels();
	MaxLevelIndex -= 1;
	BindEvents();
	Super::BeginPlay();
}

void ALevelSelectorActor::PreviousLevel()
{
	if (CurrentLevelIndex > 0)
	{
		HideAllLevels();
		CurrentLevelIndex--;
		ShowCurrentLevel();
	}
}

void ALevelSelectorActor::NextLevel()
{
	if (MaxLevelIndex > CurrentLevelIndex)
	{
		HideAllLevels();
		CurrentLevelIndex++;
		ShowCurrentLevel();
	}
}

void ALevelSelectorActor::TogglePanel()
{
}

void ALevelSelectorActor::SetupWidget(UUserWidget * NewWidget)
{
	MyWidget = NewWidget;
	Super::BeginPlay();
}

void ALevelSelectorActor::HideAllLevels()
{
	for (ALevelActor* Level : Levels)
	{
		Level->SetActorHiddenInGame(true);
	}
}

void ALevelSelectorActor::ActivateUI()
{
	if (MyWidget)
	{
		MyWidget->AddToViewport();
		MyWidget->bIsFocusable = true;
		MyWidget->SetKeyboardFocus();
	}
}

void ALevelSelectorActor::ShowCurrentLevel()
{
	HideAllLevels();
	ULevelSelector* LevelWidget = Cast<ULevelSelector>(MyWidget);
	ALevelActor* CurrentLevel = Levels[CurrentLevelIndex];
	CurrentLevel->SetActorHiddenInGame(false);
	CurrentLevel->SetActorRelativeLocation(CurrentLevel->UI_Pivot);
	CurrentLevel->SetActorRelativeScale3D(CurrentLevel->UI_Scale);
	LevelWidget->UpdateLevel(Levels[CurrentLevelIndex], CurrentLevelIndex);
}

void ALevelSelectorActor::SpawnAllLevels()
{
	
	FActorSpawnParameters SpawnParams;
	for (TSubclassOf<ALevelActor> Level : LevelsToChoose)
	{
		
		MaxLevelIndex++;
		ALevelActor* NewLevel = GetWorld()->SpawnActor<ALevelActor>(Level, LevelsRoot->GetComponentLocation(), LevelsRoot->GetComponentRotation(), SpawnParams);
		
		NewLevel->SetActorHiddenInGame(true);
		NewLevel->AttachToComponent(LevelsRoot, FAttachmentTransformRules::SnapToTargetIncludingScale);
		Levels.Add(NewLevel);

	}
	if (Levels.Num() > 0)
		ShowCurrentLevel();
}

void ALevelSelectorActor::BindEvents()
{
	ULevelSelector* LevelWidget = Cast<ULevelSelector>(MyWidget); 
	LevelWidget->OnNextLevel.AddDynamic(this, &ALevelSelectorActor::NextLevel);
	LevelWidget->OnPreviousLevel.AddDynamic(this, &ALevelSelectorActor::PreviousLevel);
}

void ALevelSelectorActor::BindOnNearLevel()
{
}

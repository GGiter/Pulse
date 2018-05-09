// Fill out your copyright notice in the Description page of Project Settings.

#include "Terminal.h"
#include "Pulse.h"

#include "UI/TerminalUI.h"
#include "PlayerCharacter.h"


void ATerminal::BeginPlay()
{
	Super::BeginPlay();
	if (WidgetClass)
	{
		TerminalWidget = CreateWidget<UTerminalUI>(GetWorld(),WidgetClass);
	}
}
void ATerminal::OnInteract_Implementation(AActor*Caller)
{
	BP_OnInteract(Caller);
}
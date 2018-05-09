// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponSelection.h"
#include "Pulse.h"



void UWeaponSelection::CallOnClickedBack()
{
	OnClickedBack.Broadcast();
}


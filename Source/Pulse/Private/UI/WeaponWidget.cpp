// Fill out your copyright notice in the Description page of Project Settings.

#include "WeaponWidget.h"
#include "Pulse.h"



void UWeaponWidget::CallOnClick(FBackpackItem WeaponInfo, bool isShowingInBackpack)
{
	OnClick.Broadcast(WeaponInfo,isShowingInBackpack);
}

void UWeaponWidget::SetHoldingWeapon(FBackpackItem New)
{
	HoldingWeapon = New;
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "UIWeaponBackpack.h"
#include "Pulse.h"


void UUIWeaponBackpack::CallOnWeaponClick(FBackpackItem Weapon, int32 WeaponSlot)
{
	OnWeaponClick.Broadcast(Weapon, WeaponSlot);
}

void UUIWeaponBackpack::SetWeaponSlot(int32 New)
{
	WeaponSlot = New;
}

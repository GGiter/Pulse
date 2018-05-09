// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UIWeaponBackpack.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponClick, FBackpackItem, Weapon, int32, WeaponSlot);

UCLASS()
class PULSE_API UUIWeaponBackpack : public UUserWidget
{
	GENERATED_BODY()
public:
		UPROPERTY(BlueprintAssignable, Category = Delegates)
		FOnWeaponClick OnWeaponClick;
		UFUNCTION(BlueprintCallable, Category = Delegates)
			void CallOnWeaponClick(FBackpackItem Weapon, int32 WeaponSlot);
		UPROPERTY(BlueprintReadWrite)
			int32 WeaponSlot;
		UFUNCTION(BlueprintCallable)
			void SetWeaponSlot(int32 New);
	
	
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"
#include "WeaponWidget.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnClick, FBackpackItem, WeaponInfo, bool, isShowingInBackpack);

UCLASS()
class PULSE_API UWeaponWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(BlueprintAssignable, Category = Delegates)
		FOnClick OnClick;
	UFUNCTION(BlueprintCallable, Category = Delegates)
		void CallOnClick(FBackpackItem WeaponInfo,bool isShowingInBackpack);
	UFUNCTION(BlueprintCallable)
		void SetHoldingWeapon(FBackpackItem New);
	UPROPERTY(BlueprintReadWrite)
	FBackpackItem HoldingWeapon;
	UPROPERTY(BlueprintReadWrite)
	bool isShowingInBackpack;
	
};

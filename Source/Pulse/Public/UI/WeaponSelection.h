// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Pulse.h"
#include "Blueprint/UserWidget.h"
#include "WeaponSelection.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedBack);

UCLASS()
class PULSE_API UWeaponSelection : public UUserWidget
{
	GENERATED_BODY()
public:
		UPROPERTY(BlueprintReadWrite)
		TArray<FBackpackItem> SelectedWeapons;
		UPROPERTY(BlueprintAssignable, Category = Delegates)
		FOnClickedBack OnClickedBack;
	UFUNCTION(BlueprintCallable, Category = Delegates)
		void CallOnClickedBack();
	
	
};

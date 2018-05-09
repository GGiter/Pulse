// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactable.h"
#include "Terminal.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ATerminal : public AInteractable
{
	GENERATED_BODY()
public:
	virtual void BeginPlay() override;
	virtual void OnInteract_Implementation(AActor*Caller) override;
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void BP_OnInteract(AActor* Caller);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Shop)
		TArray < TSubclassOf<class AWeaponBase> >WeaponShopArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Shop)
		TArray < TSubclassOf<class AVehicleBase> >VehicleShopArray;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Shop)
		TArray < TSubclassOf<class APickupBase> >PickupShopArray;
private:
	UPROPERTY(Category = Widget, EditDefaultsOnly)
		TSubclassOf<class UTerminalUI> WidgetClass;
	UPROPERTY(Category = Widget,BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UTerminalUI *TerminalWidget;
	
	
	
};

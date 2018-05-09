// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "VehicleBase.h"
#include "VehicleController.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AVehicleController : public APlayerController
{
	GENERATED_BODY()
	void BeginPlay() override;
	void SetupInputComponent() override;
	
public:
	
	void MoveX(float Value);
	void SetMoveable(bool bNewMoveable);
	void MoveY(float Value);
	void HandbrakeReleased();
	void HandbrakePressed();
	void ChangeView();
	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void NitroOn();
	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void NitroOff();

	UPROPERTY(BlueprintReadOnly)
		AVehicleBase* VehicleRef;
private:
	bool bMoveable = false;
};

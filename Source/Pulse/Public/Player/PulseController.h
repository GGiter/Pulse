// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PulseController.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API APulseController : public APlayerController
{
	GENERATED_BODY()
		void BeginPlay() override;
		void SetupInputComponent() override;
public:
		void MoveX(float Value);
		void MoveY(float Value);
		void LookPitch(float Value);
		void LookYaw(float Value);
		UFUNCTION(BlueprintCallable, WithValidation,Server, Reliable, Category = PlayerAbility)
		void Jump();
		UFUNCTION(BlueprintCallable,WithValidation,NetMulticast,Unreliable,Category=PlayerAbility)
		void Use();
		UFUNCTION(BlueprintCallable,  Category = PlayerAbility)
		void StartFiring();
		UFUNCTION(BlueprintCallable,  Category = PlayerAbility)
		void StopFiring();
		void HandbrakeReleased();
		UFUNCTION(BlueprintCallable,WithValidation,Server,Reliable)
			void CloseCombatAttack();
		UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void SprintOn();
		UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void SprintOff();
		UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void CrouchToggle();
		UFUNCTION(BlueprintCallable, Category = PlayerAbility)
		void Reload();
		UPROPERTY(BlueprintReadOnly)
		class APlayerCharacter* PlayerRef;
		class UUserWidget* PlayerHUD;
		UPROPERTY(BlueprintReadOnly)
		class ATurret* TurretRef;
		UFUNCTION(BlueprintCallable, Category = PlayerAbility)
		void SwitchToFirstSlot();
		UFUNCTION(BlueprintCallable, Category = PlayerAbility)
		void SwitchToSecondSlot();
		UFUNCTION(BlueprintCallable, Category = PlayerAbility)
		void SwitchToThirdSlot();
		void ZoomIn();
		void ChangeView();
		void Throw();
		void QuitUI();
		
public:
	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class UUserWidget> HUDClass;
private:
	
	
};

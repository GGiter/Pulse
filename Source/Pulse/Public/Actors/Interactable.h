// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Interactable.generated.h"

UCLASS(ABSTRACT)
class PULSE_API AInteractable : public AActor
{
	GENERATED_BODY()
	

public:

	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor*Caller);
	void OnBeginFocus();
	void OnEndFocus();
	
	AInteractable();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* MeshComp;
protected:
	UPROPERTY(EditDefaultsOnly)
		bool bCanInteract;



	
};

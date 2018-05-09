// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "IntermissionNode.generated.h"

UCLASS()
class PULSE_API AIntermissionNode : public AActor
{
	GENERATED_BODY()
	
public:
	AIntermissionNode();

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
		USceneComponent* SceneComp;

	UPROPERTY(VisibleAnywhere)
		UBoxComponent* BoxComp;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* SMComp;
private:
	UFUNCTION()
		void OnBoxOverlap(class UPrimitiveComponent* HitComp, AActor* OverlapedActor, UPrimitiveComponent* OverlapedComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void OnBoxEndOverlap(class UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);


	
	
};

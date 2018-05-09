// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelActor.generated.h"

UCLASS()
class PULSE_API ALevelActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelActor(const FObjectInitializer& ObjectInitializer);
	UPROPERTY(EditDefaultsOnly)
		FVector UI_Pivot;
	UPROPERTY(EditDefaultsOnly)
		FVector UI_Scale;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* LevelMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Proporties)
		FName LevelName = "Default Name";
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};

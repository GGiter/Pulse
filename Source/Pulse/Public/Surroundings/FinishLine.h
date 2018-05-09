// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "FinishLine.generated.h"

UCLASS()
class PULSE_API AFinishLine : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFinishLine();
	UFUNCTION()
		void LineCrossed(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex,bool bFromSweep,const FHitResult &SweepResult);
	UFUNCTION(WithValidation, Server, Reliable)
		void LineCrossedServer(AVehicleBase* Player);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		UBoxComponent* BoxCollision;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAccess = "true"))
		FVector BoxExtend = { 800.0f,800.0f,800.0f };
	
	
};

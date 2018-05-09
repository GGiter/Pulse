// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "PickupBase.generated.h"

UCLASS()
class PULSE_API APickupBase : public AActor
{
	GENERATED_BODY()
	
public:

	APickupBase();

	UFUNCTION(BlueprintNativeEvent, Category = Interaction)
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName PickupName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* ObjectImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ObjectPrice;
	void SetCanInteract(bool NewInteract);
	UFUNCTION()
		virtual void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void RespawnPickup();

	virtual void OnPickedUp();

	virtual void OnRespawned();

protected:
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* PickupEffect;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* PickupMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USphereComponent* CollisionSphere;
	UPROPERTY(EditDefaultsOnly)
		bool bCanInteract ;
	UPROPERTY(EditDefaultsOnly)
		bool bTouchInteracts;
	UPROPERTY(EditDefaultsOnly)
		bool bCanRespawn;

		bool bIsActive;
	UPROPERTY(EditDefaultsOnly)
		float RespawnDelayRange;
	UPROPERTY(EditDefaultsOnly)
		float RespawnDelay;
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* PickupSound;

	
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Pickups/PickupBase.h"
#include "Grenade.generated.h"

UCLASS()
class PULSE_API AGrenade : public APickupBase
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrenade();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		URadialForceComponent* RadialForce;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MinWeaponDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float MaxWeaponDamage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float CritDamageModifier;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float DamageRadius;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		FName GrenadeName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		float Price;
	UPROPERTY(EditDefaultsOnly)
		USoundWave* ExplodeSound;
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* ExplodeEffect;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SpawnExplodeEffect();
	void Explode(const FHitResult &Result);
	void ExplodeAfterTime();
	void OnStop(const FHitResult &ImpactResult);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetOwningCharacter(ACharacter* NewChar);

private:
	FTimerHandle ExplodeTimer;
	UPROPERTY(EditDefaultsOnly)
	float ExplodeDelay;
	UPROPERTY(EditDefaultsOnly)
		bool bExplodesAfterTime;
	
	UPROPERTY(EditDefaultsOnly)
	bool bIsCausingRadiusDamage;

	ACharacter* OwningCharacter;
};

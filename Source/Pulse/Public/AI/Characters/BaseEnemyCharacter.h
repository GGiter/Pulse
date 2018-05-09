// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Components/WidgetComponent.h"
#include "BaseEnemyCharacter.generated.h"

UCLASS()
class PULSE_API ABaseEnemyCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	/*The last time the last attack occured*/
	float LastAttack;
	UPROPERTY(EditDefaultsOnly)
		int32 MoneyReward;
	virtual void Tick(float DeltaTime) override;
public:
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
		UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* DamageTakenCue;
	class UBaseEnemyAnimInstance* AnimInstance;
	UPROPERTY(EditAnywhere, Category = "Death")
		UMaterialInterface* DeathMat;
	UPROPERTY(EditAnywhere, Category = "Combat")
		float BaseDamageAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Setup)
		FName EnemyName;
	UPROPERTY(EditAnywhere, Category = "Combat",BlueprintReadOnly)
		float Health;
	UPROPERTY(EditAnywhere, Category = "Combat", BlueprintReadOnly)
		float MaxHealth;
	/*This value is used in order to avoid spamming attacks when the player is firing. A value of 1 means that the
	enemy can be damaged one time per second*/
	UPROPERTY(EditAnywhere, Category = "Combat")
		float SpamAttack = 1.f;

	UPROPERTY(EditAnywhere, Category = "Combat")
		int32 XpReward;
	UPROPERTY(VisibleAnywhere, Category = "AI")
		class UPawnSensingComponent* PawnSensingComp;
	UPROPERTY(EditDefaultsOnly)
		float MaxRange;
	virtual void EnableHunting(APawn* Pawn);
	void SetControllerDistance(float Distance);
	UFUNCTION(BlueprintCallable)
		bool IsRunning() const;
	UFUNCTION(BlueprintCallable)
		FRotator GetAimOffsets() const;
	UFUNCTION(BlueprintPure,Category="Combat")
		FORCEINLINE bool GetIsAttacked() const
	{
		return bIsAttacked;
	}

public:
	// Sets default values for this character's properties
	ABaseEnemyCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//Combat


	virtual void Attack();

	virtual float TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
	virtual void OnDeath(AActor* Caller);

	inline bool IsAlive() { return Health > 0; }

	FORCEINLINE int32 GetMoneyReward() { return MoneyReward; }
	FORCEINLINE int32 GetXpReward() { return XpReward; }

	/*Behavior Tree for the current character*/
	UPROPERTY(EditAnywhere, Category = "AI")
		class UBehaviorTree* BehaviorTree;
private:
	bool bWantsToRun;
	bool bWantsToRunToggled;
protected:
	UPROPERTY(EditAnywhere, Category = "Combat", meta = (AllowPrivateAcess = "true"))
		float HealthDelayTime;
		bool bIsAttacked = false;
	UPROPERTY(EditAnywhere, Category = "Death", meta = (AllowPrivateAcess = "true"))
		float EndDeathTime;
		float DeathTime=0.0f;
	FTimerHandle DeathTimerHandle;
	FTimerHandle HealthDelayTimerHandle;
	UMaterialInstanceDynamic* DeathDynMat;
	void StartDeathAnim();
	void DeathAnim();
	void StopHealthWidget();
	virtual void DestroyWeapon();
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Turret.generated.h"

UCLASS()
class PULSE_API ATurret : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	ATurret();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor*Caller);
	void MoveGuns(float Value);
	void MoveMainBody(float Value);
	void StartFire();
	void StopFire();
	void OnBeginFocus();
	void OnEndFocus(); 
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* LeftGunMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* RightGunMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MainBodyMesh;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;
	class APlayerCharacter* Player;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ActivateWidgets();
	void DeactivateWidgets();
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UFUNCTION(BlueprintPure, Category = PlayerStatus)
		FORCEINLINE float GetCurrentAmmo() {
		return CurrentAmmo;
	}
	UFUNCTION(BlueprintPure, Category = PlayerStatus)
		FORCEINLINE float GetMaxAmmo() {
		return MaxAmmo;
	}
	UFUNCTION(BlueprintPure, Category = PlayerStatus)
		FORCEINLINE bool IsAlive() {
		return CurrentHealth > 0;
	}
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> PlayerHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* PlayerHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> WaveHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* WaveHUD;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> ExitUIClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* ExitUI;

	bool bIsInQuitUI = false;

	UFUNCTION(BlueprintPure, Category = Stats)
		FORCEINLINE float GetMaxHealth() {
		return MaxHealth;
	}
	UFUNCTION(BlueprintPure, Category = Stats)
		FORCEINLINE float GetCurrentHealth() {
		return CurrentHealth;
	}

	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void Spot_On();
	void UpdateHUD();
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void BP_UpdateHUD();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* DamageTakenCue;
	void AddBossUI(UUserWidget * NewBossUI);
	void EnterQuitUI();
	void ExitQuitUI();
private:
	void SimRecoil();
	void DoFire();
	void DealDamage(const FHitResult& Hit);
	FVector CalcSpread();
	void SpawnFireEffect();
	void SpawnImpactEffect(const FHitResult& Hit);
	void SpawnBullet();
	FVector AimDir();
	void OnDeath();
	FCollisionQueryParams TraceParams;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MainBodyRotationSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float GunsRotationSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxAmmo = 200;
		float StartingAmmo;
		float CurrentAmmo;
	UPROPERTY(EditDefaultsOnly, Category = Spread)
		float SpreadIncreaseSpeed=1.0f;
	UPROPERTY(EditDefaultsOnly, Category = Spread)
		float SpreadDecreaseSpeed = 1.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float BaseDamage=20.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float FireRate=10.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxRange=3000.0f;
	UPROPERTY(EditDefaultsOnly)
		UParticleSystem* ShotEffect;
	UPROPERTY(EditDefaultsOnly)
		USoundWave* ShotSound;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		UParticleSystem* ImpactEffect;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		USoundWave* ImpactSound;

	UPROPERTY(BlueprintReadOnly, Category = Spread, meta = (AllowPrivateAccess = "true"))
		float Spread=1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spread, meta = (AllowPrivateAccess = "true"))
		float MinSpread=1.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Spread, meta = (AllowPrivateAccess = "true"))
		float MaxSpread=1.0f;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<class ABullet> BulletClass;

	UPROPERTY(EditDefaultsOnly)
		float RechargeAmount;
		float Charge;
	bool bIsFiring;
	FTimerHandle FireRateHandle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* RightMuzzle;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USceneComponent* LeftMuzzle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent *CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* TPSCamera;

	USceneComponent* CurrentMuzzle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* PlayerNameRender;
	
	UPROPERTY(EditDefaultsOnly,Replicated, Category = Stats)
		float MaxHealth=200.0f;
	UPROPERTY(Replicated)
		float CurrentHealth;

	UUserWidget* BossUI;
	bool bLeftGunRecoil = false;
	bool bRightGunRecoil = false;
	float RecoilCheckLeft = 0.0f;
	float RecoilCheckRight = 0.0f;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Pulse.h"
#include "Turret/Turret.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class PULSE_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UAudioComponent* AudioComp;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* DamageTakenCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* SprintCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ChangeWeaponCue;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* ReloadCue;
	UPROPERTY(EditDefaultsOnly, Category = Throw)
		float ThrowStrength;
	UFUNCTION(BlueprintPure, Category = Stats)
		FORCEINLINE float GetMaxHealth() {
		return MaxHealth;
	}
	UFUNCTION(BlueprintPure, Category = Stats)
		FORCEINLINE float GetCurrentHealth() {
		return CurrentHealth;
	}
	UFUNCTION(BlueprintPure,Category=Stats)
		FORCEINLINE float GetMaxStamina() {
		return MaxStamina;
	}
	UFUNCTION(BlueprintPure,Category=Stats)
		FORCEINLINE float GetCurrentStamina() {
		return CurrentStamina;
	}
	UFUNCTION(BlueprintPure, Category = Stats)
		FORCEINLINE float GetMaxShield() {
		return MaxShield;
	}
	UFUNCTION(BlueprintPure,Category=Stats)
		FORCEINLINE float GetCurrentShield() {
		return CurrentShield;
	}
	inline float GetRunSpeed() {
		return RunSpeed;
	}
	inline float GetWalkSpeed() {
		return WalkSpeed;
	}
	UFUNCTION(BlueprintCallable, Category = Stats)
		void UpdateCurrentHealth(float Amount);
	UFUNCTION(BlueprintCallable, Category = Stats)
		void UpdateCurrentShield(float Amount);
	UFUNCTION(BlueprintCallable, Category = Grenade)
		void UpdateGrenadeAmount(int Amount);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent *CameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
		UTextRenderComponent* PlayerNameRender;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		USpringArmComponent *TPSCameraBoom;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* FPSCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ZoomInCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* TPSCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* ZoomInTPSCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* TPSMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* TP_Weapon;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* FPSMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class AGrenade> GrenadeClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
		TSubclassOf<class ASideArmsBase> CloseCombatWeaponClass;
	bool bInZoom;
	bool bInFPS;
	UPROPERTY(EditDefaultsOnly,BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> WaveHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* WaveHUD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> PlayerHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> DeathUIClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> ExitUIClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* PlayerHUD;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* DeathUI;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* ExitUI;
		
	void EnterQuitUI();
	UFUNCTION(BlueprintCallable,Category=UI)
	void ExitQuitUI();
	bool bIsInQuitUI = false;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, FName> WeaponAttachPointMapFPS;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, FName> WeaponAttachPointMapTPS;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, UAnimMontage*> WeaponReloadAnimMap;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, UAnimMontage*> WeaponFireAnimMap;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, UAnimMontage*> TPPWeaponReloadAnimMap;
	UPROPERTY(Category = MapsAndSets, EditAnywhere)
	TMap<EWeaponType, UAnimMontage*> TPPWeaponFireAnimMap;
	void PlayReloadAnim();
	UPROPERTY(EditDefaultsOnly,Category=AnimMontages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = AnimMontages)
	UAnimMontage* SecondAttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = AnimMontages)
	UAnimMontage* ThirdAttackMontage;
	void SetPlayerTurret(ATurret* NewPlayerTurret);

	void ActivateWidgets();
	void DeactiveWidgets();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void AddBossUI(UUserWidget* NewBossUI);
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	bool bIsMovementEnabled;
	bool bIsJumping;
	UPROPERTY(Replicated)
	bool bIsRunning;
	UPROPERTY(Replicated)
	bool bIsCrouching;
	UFUNCTION(BlueprintPure, Category = Anim)
		FORCEINLINE bool GetIsJumping() {
		return bIsJumping;
	}
	UFUNCTION(BlueprintPure, Category = Anim)
		FORCEINLINE bool GetIsRunning() {
		return bIsRunning;
	}
	UFUNCTION(BlueprintPure, Category = Anim)
		FORCEINLINE bool GetIsCrouching() {
		return bIsCrouching;
	}
	UFUNCTION(BlueprintPure, Category = Weapons)
		FORCEINLINE int32 GetGrenadeAmount()
	{
		return GrenadeAmount;
	}
		void UpdateHUD();
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void BP_UpdateHUD();
	void SetFov(float NewFov);
	void ThrowGrenade();
	void CloseCombatAttack();
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void Spot_On();
	UFUNCTION(BlueprintImplementableEvent, Category = HUD)
		void Item_PopUp(FName ItemName);
	UFUNCTION(BlueprintCallable,Category=Anim)
	void ChainCombo();
	UFUNCTION(BlueprintCallable, Category = Anim)
	void ComboReset();
private:
	
	
	bool bIsMeleeAttacking=false;
	bool bChainAttack=false;
	int32 Combo=1;
	
	void HandleCombo();
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxHealth;
	UPROPERTY(Replicated)
		float CurrentHealth;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxStamina;
	UPROPERTY(Replicated)
		float CurrentStamina;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float MaxShield;
	UPROPERTY(Replicated)
		float CurrentShield;
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float JumpHeight=800.0f;
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float RunSpeed=1200.0f;
	UPROPERTY(EditDefaultsOnly, Category = Movement)
		float WalkSpeed=500.0f;
	UPROPERTY(EditDefaultsOnly, Category = Stats)
		float InteractionDistance;


	bool bHasNewFocus;
	bool bTurretHasNewFocus;
	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AWeaponBase>> StartingWeapons;
	FCollisionQueryParams TraceParams;
	UUserWidget* BossUI;
	bool bOffset;
	void HandleHighlight();
	void HandleActorRotation(float DeltaTime);
	class AInteractable* FocusedActor;
	ATurret* FocusedTurret;
	ATurret* PlayerTurret;
	bool bIsTakingDamage;
	void ChangeTP_WeaponMesh();
	UFUNCTION(BlueprintCallable)
	void SetupStartingWeapons();
	class ASideArmsBase* CloseCombatWeapon;
	void SimGrenade();
	
public:
	class AInteractable* FindFocusedActor();
	ATurret* FindFocusedTurret();
	void Heal(float Value);
public:
	UPROPERTY(EditDefaultsOnly, Category = Weaponary,Replicated)
		FName FPSWeaponSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName TPSWeaponSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName FPSSniperSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName TPSniperSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName PistolFPSWeaponSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName PistolTPSWeaponSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName CloseCombatSocketName;
	UPROPERTY(EditDefaultsOnly, Category = Weaponary, Replicated)
		FName GrenadeSocketName;
	UPROPERTY(Replicated)
	bool bHasGun;
	UPROPERTY(EditDefaultsOnly)
		int32 GrenadeAmount;
	void EquipWeapon(class AWeaponBase* WeaponToEquip,int32 Slot);
	UFUNCTION(BlueprintCallable)
	void AddToInventory(class AWeaponBase* NewWeapon,int32 Slot);
	UFUNCTION(BlueprintCallable)
	int32 SetWeaponSlot(class AWeaponBase* Weapon);
	UFUNCTION(BlueprintPure, Category = Weapon)
		FORCEINLINE class AWeaponBase* GetCurrentWeapon() {
		return CurrentWeapon;
	}

	
	UFUNCTION(BlueprintPure,Category=Weapon)
		FORCEINLINE bool HasGun() {
		return bHasGun;
	}
	
	UFUNCTION(BlueprintPure, Category = PlayerStatus)
	FORCEINLINE bool IsAlive() {
		return CurrentHealth > 0; }
public:
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor * DamageCauser) override;
	void OnDeath();
	void ZoomInHUD();
	void ZoomOutHUD();
	UUserWidget* ZoomInHUDRef;
	UPROPERTY(BlueprintReadWrite)
	TArray<FBackpackItem> Backpack;
	UPROPERTY(BlueprintReadOnly,Replicated)
	class AWeaponBase* CurrentWeapon;
	int32 CurrentSlot;
	void AddAmmo(EAmmoType AmmoType, float Amount);
	void ChangeWeaponAttachment(class AWeaponBase* Weapon);
};


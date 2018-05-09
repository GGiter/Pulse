// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "WheeledVehicle.h"
#include "WheeledVehicleMovementComponent4W.h"
#include "VehicleBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API AVehicleBase : public AWheeledVehicle
{
	GENERATED_BODY()
	
	/** Spring arm that will offset the camera */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* SpringArm;

	/** Camera component that will be our viewpoint */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* Camera;
	/** SCene component for the In-Car view origin */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* InternalCameraBase;

	/** Camera component for the In-Car view */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UCameraComponent* InternalCamera;

	/** Text component for the In-Car speed */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarSpeed;

	/** Text component for the In-Car gear */
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		UTextRenderComponent* InCarGear;

	/** Audio component for the engine sound */
	

public:
	AVehicleBase();
	UPROPERTY(Category = Sound, VisibleDefaultsOnly, BlueprintReadOnly)
		UAudioComponent* NitroAudioComponent;
	UPROPERTY(Category = Sound, VisibleDefaultsOnly, BlueprintReadOnly)
		UAudioComponent* EngineSoundComponent;
	UPROPERTY(Category = Sound, VisibleDefaultsOnly, BlueprintReadOnly)
		UAudioComponent* BreakAudioComponent;
	UPROPERTY(Category = Sound, VisibleDefaultsOnly, BlueprintReadOnly)
		UAudioComponent* HandbrakeAudioComponent;
	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText SpeedDisplayString;

	UPROPERTY(Category = Display, VisibleDefaultsOnly, BlueprintReadOnly)
		FText GearDisplayString;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadWrite)
		FColor	GearDisplayColor;

	UPROPERTY(Category = Display, EditAnywhere, BlueprintReadWrite)
		FColor	GearDisplayReverseColor;

	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInCarCameraActive;

	/** Initial offset of incar camera */
	FVector InternalCameraOrigin;


	/** Are we in reverse gear */
	UPROPERTY(Category = Camera, VisibleDefaultsOnly, BlueprintReadOnly)
		bool bInReverseGear;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FName VehicleName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* VehicleImage;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float VehiclePrice;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float NitroSpeed=1.04f;
	UPROPERTY(BlueprintReadOnly)
		class APlayerCharacter* Player;
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = Interaction)
		void OnInteract(AActor* Caller);
	virtual void OnInteract_Implementation(AActor* Caller);
	void OnBeginFocus();
	void OnEndFocus();
	void OnToggleCamera();
	void SetupInCarHUD();
	void PlayHandbrakeSound();
	void StopHandbrakeSound();
	UPROPERTY(EditDefaultsOnly,Category=Lights)
		UMaterialInterface* BreakLightsMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Lights)
		UMaterialInterface* ReverseLightsMaterial;
	UPROPERTY(EditDefaultsOnly, Category = Lights)
		float  BaseLightGlow = 10.0f;
	UPROPERTY(EditDefaultsOnly, Category = Lights)
		float InUseLightGlow = 120.0f;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* NitroSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* EngineSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* BreakSound;
	UPROPERTY(EditDefaultsOnly, Category = Sound)
		USoundCue* HandbreakSound;
	UMaterialInstanceDynamic* BreakMaterialRef;
	UMaterialInstanceDynamic* ReverseBreakMaterialRef;
	UFUNCTION(NetMulticast,WithValidation,Reliable)
	void SetNitroOn();
	UFUNCTION(NetMulticast, WithValidation, Reliable)
	void SetNitroOff();
	// Begin Actor interface
	void BreakOn();
	void BreakOff();
	virtual void Tick(float Delta) override;
	UFUNCTION(BlueprintCallable)
		void SetFinishLocation(FVector Location);
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetRaceTime()
	{
		return RaceTime;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetKPH()
	{
		return KPH;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE int32 GetGear()
	{
		return Gear;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE bool GetInReverseGear()
	{
		return bInReverseGear;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE FVector GetFinishLocation()
	{
		return FinishLocation;
	}
	void GameEnded();
	void StartRaceCounter();
	void StopRaceCounter();
protected:
	virtual void BeginPlay() override;

public:
	// End Actor interface

	UPROPERTY(EditAnywhere, Category = Turbo)
		FVehicleEngineData TurboEngineData;
	
	UPROPERTY(EditAnywhere, Category = Turbo)
		FVehicleEngineData NonTurboEngineData;
	/** Update the physics material used by the vehicle mesh */
	void UpdatePhysicsMaterial();


	static const FName LookUpBinding;
	static const FName LookRightBinding;
	static const FName EngineAudioRPM;

	UFUNCTION()
		void IncreaseRaceTime();

private:
	UPROPERTY( VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		class UWheeledVehicleMovementComponent4W* Vehicle4W;

	/** Update the gear and speed strings */
	void UpdateHUDStrings();
	
	float RaceTime = 0.0f;
	/* Are we on a 'slippery' surface */
	bool bIsLowFriction;
	/** Slippery Material instance */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UPhysicalMaterial* SlipperyMaterial;
	/** Non Slippery Material instance */
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
	UPhysicalMaterial* NonSlipperyMaterial;

	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"))
		float KPH;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Replicated)
		int32 Gear;
	UPROPERTY(Replicated)
	float NitroAmount;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Replicated)
	float MaxNitroAmount=500.0f;
	FTimerDelegate RaceTimeTimerDelegate;
	FTimerHandle RaceTimeTimerHandle;
	bool bIsInNitro;
	bool bIsBreaking;
	bool bTimerSetup = false;
	bool bIsRacing = false;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Replicated)
		UParticleSystem* NitroEffect;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Replicated)
		FName ExhaustSocketName1 = "ExhaustSocketName1";
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAccess = "true"),Replicated)
		FName ExhaustSocketName2 = "ExhaustSocketName2";
	UPROPERTY(Replicated)
	UParticleSystemComponent* NitroEffectRef1;
	UPROPERTY(Replicated)
	UParticleSystemComponent* NitroEffectRef2;
	FVector FinishLocation = { 0.0f,0.0f,0.0f };
	void EnableIncarView(const bool bState);
public:
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetNitroAmount()
	{
		return NitroAmount;
	}
	UFUNCTION(BlueprintPure)
		FORCEINLINE float GetMaxNitroAmount()
	{
		return MaxNitroAmount;
	}
	FORCEINLINE bool GetIsInNitro() const { return bIsInNitro; }
	FORCEINLINE USpringArmComponent* GetSpringArm() const { return SpringArm; }
	FORCEINLINE UCameraComponent* GetCamera() const { return Camera; }
	FORCEINLINE UCameraComponent* GetInternalCamera() const { return InternalCamera; }
	FORCEINLINE UTextRenderComponent* GetInCarSpeed() const { return InCarSpeed; }
	FORCEINLINE UTextRenderComponent* GetInCarGear() const { return InCarGear; }
	FORCEINLINE UAudioComponent* GetEngineSoundComponent() const { return EngineSoundComponent; }
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> PlayerHUDClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = HUD)
		TSubclassOf<class UUserWidget> EndGameHUDClass;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* PlayerHUD;
	UPROPERTY(BlueprintReadOnly, Category = HUD)
		class UUserWidget* EndGameHUD;
	
};

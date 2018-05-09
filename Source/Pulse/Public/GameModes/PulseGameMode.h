// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/BossWidget.h"
#include "GameFramework/GameMode.h"
#include "PulseGameMode.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API APulseGameMode : public AGameMode
{
	GENERATED_BODY()
public:
	APulseGameMode();
	virtual void BeginPlay() override;
	UFUNCTION(BlueprintCallable, Category = Wave)
		void SetWaveDelay(float Delay);
	UFUNCTION(BlueprintCallable, Category = Wave)
		void SetMaxWaves(int32 Max);
	UFUNCTION(BlueprintCallable, Category = Wave)
		void SetWaveInfo(const TArray<FWaveInfo>& newWaveInfo);
	UFUNCTION(BlueprintCallable)
		TArray<TSubclassOf<AWeaponBase>> GetStartingWeapons() const;
	void SetSpawnDelay(float NewSpawnDelay);
	UPROPERTY(EditDefaultsOnly, Category = UI)
		TSubclassOf<UUserWidget> BossUIClass;
	void UpdateHUD();
	virtual void Killed(AController* Killer, AController* Victim);
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;
	virtual bool IsSpawnpointAllowed(class APlayerStart* SpawnPoint, AController* Player) const;
	virtual bool IsSpawnpointPreferred(class APlayerStart* SpawnPoint, AController* Player) const;
	virtual void PostLogin(APlayerController* NewPlayer) override;
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = UI, meta = (BlueprintProtected = "true"))
		TSubclassOf<class UUserWidget> PlayerHUDClass;
	UPROPERTY()
		class UUserWidget *CurrentWidget;
	void BeginWave();
	void EndWave();
	void BeginSpawning();
	void SpawnEnemy();
	virtual void StartMatch() override;
	virtual void EndMatch() override;
	virtual void InitGameState() override;
private:
	UPROPERTY(EditDefaultsOnly)
		FName SpawnTag;
	UPROPERTY(EditDefaultsOnly)
		float WaveDelay;
	UPROPERTY(EditDefaultsOnly)
		float SpawnDelay;
	UPROPERTY(EditDefaultsOnly)
		int32 MaxWaves;
	UPROPERTY(EditDefaultsOnly)
		TArray<FWaveInfo> WaveInfo;
	UPROPERTY(EditDefaultsOnly)
		TArray<TSubclassOf<AWeaponBase>> StartingWeapons;
	UPROPERTY(EditDefaultsOnly)
		bool bIsInEditor = false;
	TArray<int32> SpawnedOfType;
	int32 EnemyToSpawn;
	int32 EnemiesSpawned;

	FTimerHandle WaveTimerHandle;
	FTimerHandle SpawnTimerHandle;

	TArray<AActor*> AISpawnPoints;
	class APulseGS* PulseGS;
	
	
	
};

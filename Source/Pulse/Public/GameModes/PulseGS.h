// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "PulseGS.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API APulseGS : public AGameState
{
	GENERATED_BODY()
public:
	void SetIsWaveActive(bool newActive);
	void AddEnemiesRemaining(int32 Amount);
	void SetWaveDelay(float Delay);
	void SetMaxWaves(int32 Waves);
	void SetCurrentWave(int32 Wave);
	void LoadVariables(int32 PlayerId);
	UFUNCTION(BlueprintCallable, Category = Wave)
		FORCEINLINE  bool IsWaveActive() const { return bIsWaveActive; }
	UFUNCTION(BlueprintCallable, Category = Wave)
		FORCEINLINE  int32 GetEnemiesRemaining() const { return EnemiesRemaining; }
	UFUNCTION(BlueprintCallable, Category = Wave)
		FORCEINLINE  float GetWaveDelay() const { return WaveDelay; }
	UFUNCTION(BlueprintCallable, Category = Wave)
		void GetWaves(int32& Max, int32& Current) const;

	FORCEINLINE int32 GetMaxWaves() const { return MaxWaves; }
	FORCEINLINE int32 GetCurrentWave() const { return CurrentWave; }
	FORCEINLINE int32 GetSpawnDelay() const { return SpawnDelay; }
	FORCEINLINE TArray<struct FWaveInfo> GetWaveInfo() const { return WaveInfo; }
private:
	UPROPERTY(Replicated)
		int32 EnemiesRemaining;
	UPROPERTY(Replicated)
		bool bIsWaveActive;
	UPROPERTY(Replicated)
		float  WaveDelay;
	UPROPERTY(Replicated)
		int32 MaxWaves;
	UPROPERTY(Replicated)
		int32 CurrentWave=0;
	UPROPERTY(Replicated)
		float SpawnDelay;
	UPROPERTY(Replicated)
		TArray<struct FWaveInfo> WaveInfo;

	
	
	
};

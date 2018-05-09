// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#define STENCIL_FRIENDLY_OUTLINE 252;
#define STENCIL_NEUTRAL_OUTLINE 253;
#define STENCIL_ENEMY_OUTLINE 254;
#define STENCIL_ITEMHIGHLIGHT 255;
#define ECC_Weapon ECC_GameTraceChannel1

#include "Engine.h"
#include "UnrealNetwork.h"
#include "Online.h"
#include "Pulse.generated.h"



USTRUCT(BlueprintType)
struct FBackpackItem
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		class AWeaponBase* Weapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		class UMaterial* Material;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		int32 Slot;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items)
		bool bIsSelected;
	inline bool operator==(const FBackpackItem& f) const
	{
		return Weapon == f.Weapon;
	}
};
UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	AT_Bullets UMETA(DisplayName = "Bullets"),
	AT_Sniper UMETA(DisplayName="Sniper"),
	AT_Pistol UMETA(DisplayName="Pistol"),
	AT_Rockets UMETA(DisplayName = "Rockets"),
	AT_Lasers UMETA(DisplayName = "Lasers"),
	AT_Shotgun UMETA(DisplayName="Shotgun"),
	AT_GrenadeLaucher UMETA(DisplayName="GrenadeLauncher"),
	AT_Grenades UMETA(DisplayName="Grenades")
};
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	ET_AssaultRifle UMETA(DisplayName="AssaultRifle"),
	ET_Pistol UMETA(DisplayName="Pistol"),
	ET_RocketLauncher UMETA(DisplayName="RocketLauncher"),
	ET_SniperRifle UMETA(DisplayName="SniperRifle"),
	ET_LaserRifle UMETA(DisplayName="LaserRifle"),
	ET_Shotgun UMETA(DisplayName="Shotgun"),
	ET_GrenadeLauncher UMETA(DisplayName="GrenadeLauncher")

};
USTRUCT(BlueprintType)
struct FSpawnInfo
{
	GENERATED_USTRUCT_BODY()
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		TSubclassOf<AActor> EnemyClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		int32 MaxAmount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		float Probability;
};
USTRUCT(BlueprintType)
struct  FWaveInfo
{
	GENERATED_USTRUCT_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		int32 TotalNumberOfEnemies;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Spawning)
		TArray<FSpawnInfo> SpawnInfo;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Interactable.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"
#include "SideArmsBase.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API ASideArmsBase : public AInteractable
{
	GENERATED_BODY()
	
public:
	ASideArmsBase();
	
	void DoDamage();
	virtual void Tick(float DeltaTime) override;
	UFUNCTION(Server,WithValidation,Reliable)
	void DoDamageServer(const FHitResult& Hit);
	void ChangeOnwer(class APlayerCharacter* Player);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
		USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite)
		FName WeaponName;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UTexture2D* WeaponImage;
	UFUNCTION(BlueprintPure,Category=Stats)
		FORCEINLINE float GetBaseDamage() const {
		return BaseDamage;
	}
	void SetIsAttacking(bool bNewIsAttacking);
private:
	UPROPERTY(EditDefaultsOnly)
		float AttackRaycastRadius = 15.0f;
	UPROPERTY(EditDefaultsOnly)
		float BaseDamage = 20.0f;
	UPROPERTY(EditDefaultsOnly)
		FName LowerSphereSocket;
	UPROPERTY(EditDefaultsOnly)
		FName UpperSphereSocket;
	class APlayerCharacter* OwningPlayer;
	bool bIsAttacking = false;

};

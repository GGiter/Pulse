// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PULSE_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float MovementSpeed;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Angle;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsFalling ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsSprinting ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bHasGun ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bIsJumping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UAnimSequence* PullBackSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UAnimSequence* PutBackSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		UAnimSequence* FireSeq;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		FRotator RotatorDirection;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Direction;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Speed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bCrouching ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		EWeaponType WeaponType;
	UFUNCTION(BlueprintCallable, Category = "AnimationUpdates")
		void UpdateAnimationProporties();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsSecondAttackInput ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool bAcceptsThirdAttackInput ;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Pitch;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		float Yaw;

public:
	void Fire();
	
	
	
};

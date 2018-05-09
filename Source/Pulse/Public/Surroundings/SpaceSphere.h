// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "GameFramework/Actor.h"
#include "Public/Surroundings/SpaceSphereAlg.h"
#include "SpaceSphere.generated.h"

UCLASS()
class PULSE_API ASpaceSphere : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpaceSphere();
	UFUNCTION(BlueprintCallable)
		void SetStarsDensity(float NewStarsDensity);
	UFUNCTION(BlueprintCallable)
		void SetStarsBrightness(float NewStarsBrightness);
	UFUNCTION(BlueprintCallable)
		void SetNebulaScaleRange(FVector2D NewNebulaScaleRange);
	UFUNCTION(BlueprintPure)
		float GetStarsDensity() const;
	UFUNCTION(BlueprintPure)
		float GetStarsBrightness() const;
	UFUNCTION(BlueprintPure)
		FVector2D GetNebulaScaleRange() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void PostInitializeComponents() override;

	void UpdateMaterial();

		void Generate();
	UFUNCTION(BlueprintCallable)
		void ClearMaterial();
	UPROPERTY(Replicated, BlueprintReadWrite)
		UTexture2D* mDynamicTexture;

		TArray<UMaterialInstanceDynamic*> mDynamicMaterials;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
		UStaticMeshComponent* StaticMeshComponent;
private:

	
	
	UPROPERTY(EditAnywhere)
		UMaterialInterface* BaseMaterial;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Replicated)
	UTexture2D* textureToReadFrom;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category = Galaxy,Replicated)
		float StarsDensity=0.003f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category = Galaxy,Replicated)
		float StarsBrightness = 0.18f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Category=Galaxy,Replicated)
		FVector2D NebulaScaleRange = { 0.001f,0.003f };

	uint32 mDataSize;

	uint32 mDataSqrtSize;
	
	FUpdateTextureRegion2D* mUpdateTextureRegion;
	
	uint8* mDynamicColors;
	
	uint8* mTextureColors;

	
		
	bool bMaterialSet = false;
	
	
	bool bC = true;
	UPROPERTY(EditDefaultsOnly,Replicated)
	bool isSetup = false;

	int32 w, h;
};

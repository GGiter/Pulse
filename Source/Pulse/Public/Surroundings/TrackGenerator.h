// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TrackAlg.h"
#include "Classes/Components/SplineComponent.h"
#include "Components/SplineMeshComponent.h"
#include "Net/UnrealNetwork.h"
#include "TrackGenerator.generated.h"

UCLASS()
class PULSE_API ATrackGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrackGenerator();
	UFUNCTION(BlueprintCallable, WithValidation, Server, Reliable, Category = PlayerAbility)
		void Generate();
	UFUNCTION(BlueprintCallable)
		void SetSteepness(float NewSteepness);
	UFUNCTION(BlueprintCallable)
		void SetNewSpacing(float NewSpacing);
	UFUNCTION(BlueprintCallable)
		void SetNumberOfPoints(int32 NewNumberOfPoints);
	UFUNCTION(BlueprintCallable)
		void SetEllipseWidth(float NewEllipseWidth);
	UFUNCTION(BlueprintCallable)
		void SetEllpiseHeight(float NewEllipseHeight);
	UFUNCTION(BlueprintPure)
		float GetSteepness() const;
	UFUNCTION(BlueprintPure)
		float GetSpacing() const;
	UFUNCTION(BlueprintPure)
		int32 GetNumberOfPoints() const;
	UFUNCTION(BlueprintPure)
		float GetEllipseWidth() const;
	UFUNCTION(BlueprintPure)
		float GetEllipseHeight() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, meta = (AllowPrivateAcess = "true"))
		USplineComponent* PathSpline;
	UPROPERTY(BlueprintReadWrite, Replicated)
		TArray<FVector> TrackPoints;
private:
	UFUNCTION(WithValidation, Server, Reliable, Category = PlayerAbility)
	void SetupGS();
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Replicated)
		UStaticMesh* RoadMeshL;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Replicated)
		UStaticMesh* RoadMeshNL;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"), Replicated)
		UStaticMesh* StartRoadMesh;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Replicated)
		UStaticMesh* EndRoadMesh;
	
	TArray<TArray<TArray<FVector> > > SurroundingsHM;
	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAcess = "true"))
	USphereComponent* SphereComponent;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		FVector RoadScale = { 4.0f,4.0f,4.0f };
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		float Steepness=0.5f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		float Spacing=800.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		float TangentScale=1.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		bool bAutoSpacing = false;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"), Replicated)
		float StartingHeight = 3000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"),Replicated)
		int32 NumberOfPoints=6;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"),Category=Ellipse, Replicated)
		float Ellipse_Width=30000.0f;
	UPROPERTY(EditDefaultsOnly, meta = (AllowPrivateAcess = "true"),Category=Ellipse, Replicated)
		float Ellipse_Height=30000.0f;
	UPROPERTY(EditAnywhere, meta = (AllowPrivateAcess = "true"),Replicated)
		TArray<USplineMeshComponent*> TrackMeshes;
	
	UPROPERTY(Replicated)
	bool bC = true;
	UFUNCTION(BlueprintCallable,WithValidation, Server, Reliable, Category = PlayerAbility)
	void BuildSpline();
	FVector EndLocation= { 0.0f,-200.0f,3000.0f };
	FVector StartLocation = { 0.0f,-200.0f,3000.0f };
	void BuildSplineClient();
};

// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackGenerator.h"
#include "GameModes/TrackGS.h"
#include "GameModes/TrackGM.h"




// Sets default values
ATrackGenerator::ATrackGenerator()
{
	USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootSphereComponent"));
	RootComponent = SphereComponent;
	SphereComponent->InitSphereRadius(40.0f);
	SphereComponent->SetMobility(EComponentMobility::Static);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("Path"));
	PathSpline->SetupAttachment(RootComponent);
	PathSpline->SetIsReplicated(true);
	bReplicates = true;
	
}

// Called when the game starts or when spawned
void ATrackGenerator::BeginPlay()
{
	Super::BeginPlay();

	if (!HasAuthority())
	{
		BuildSplineClient();
		
	
		
	}
	
}

// Called every frame
void ATrackGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (FTrackAlg::IsThreadFinished() && bC == false)
	{
		bC = true;
		
		BuildSpline();
		SetupGS();
		BuildSplineClient();
		FTrackAlg::Shutdown();
	}
}
void ATrackGenerator::BuildSplineClient()
{
	/**/
	for (USplineMeshComponent* SplineMesh : TrackMeshes)
	{
		//SplineMesh->DestroyComponent();
	}
	TrackMeshes.Empty();
	PathSpline->ClearSplinePoints();
	for (FVector Vector : TrackPoints)
	{
		PathSpline->AddSplinePoint(Vector, ESplineCoordinateSpace::Local, false);
	}
	PathSpline->UpdateSpline();

	if (bAutoSpacing)
	{
		Spacing = RoadMeshL->GetBounds().BoxExtent.Y;
	}

	int32 Division = PathSpline->GetSplineLength() / Spacing;
	for (int32 i = 0; i < Division; ++i)
	{
		USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
		SplineMesh->SetWorldScale3D(RoadScale);
		//SplineMesh->SetIsReplicated(true);
		if (i == 0)
		{
			SplineMesh->SetStaticMesh(StartRoadMesh);
			//StartLocation = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local) + (PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local) - PathSpline->GetLocationAtDistanceAlongSpline((i + 1)*Spacing, ESplineCoordinateSpace::Local)) / 2;
		}
		else if (i == Division - 1)
		{
			SplineMesh->SetStaticMesh(EndRoadMesh);
			//EndLocation = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);;
		}
		else if (i % 4 == 0)
		{
			SplineMesh->SetStaticMesh(RoadMeshL);
		}
		else
		{
			SplineMesh->SetStaticMesh(RoadMeshNL);
		}

		FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector StartTangent = PathSpline->GetDirectionAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector EndPosB = PathSpline->GetLocationAtDistanceAlongSpline((i + 1)*Spacing, ESplineCoordinateSpace::Local);
		FVector EndTangentB = PathSpline->GetDirectionAtDistanceAlongSpline((i + 1)*Spacing, ESplineCoordinateSpace::Local);
		float Diff = FVector(StartTangent - EndTangentB).Size();
		FVector EndPos = PathSpline->GetLocationAtDistanceAlongSpline((i + 1)*Spacing + Diff * Spacing / 2, ESplineCoordinateSpace::Local);
		FVector EndTangent = PathSpline->GetDirectionAtDistanceAlongSpline((i + 1)*Spacing + Diff * Spacing / 2, ESplineCoordinateSpace::Local);
		SplineMesh->UpdateMesh();
		SplineMesh->RegisterComponent();
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
		SplineMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);


		TrackMeshes.AddUnique(SplineMesh);
	}
}
void ATrackGenerator::BuildSpline_Implementation()
{
	
	for (FVector Vector : TrackPoints)
	{
		PathSpline->AddSplinePoint(Vector, ESplineCoordinateSpace::Local, false);
	}
	PathSpline->UpdateSpline();
	
	if (bAutoSpacing)
	{
		Spacing = RoadMeshL->GetBounds().BoxExtent.Y;
	}
	
	int32 Division = PathSpline->GetSplineLength() / Spacing;
	for (int32 i = 0; i < Division; ++i)
	{
		//USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(this, USplineMeshComponent::StaticClass());
	
		//SplineMesh->SetIsReplicated(true);
		if (i == 0)
		{
			//SplineMesh->SetStaticMesh(StartRoadMesh);
			StartLocation = PathSpline->GetLocationAtDistanceAlongSpline((i + 1)*Spacing, ESplineCoordinateSpace::Local)*RoadScale;
			
		}
		else if (i == Division - 1)
		{
			//SplineMesh->SetStaticMesh(EndRoadMesh);	
			EndLocation = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local)*RoadScale+FVector(0.0f,0.0f,100.0f);
			
		}
		else if (i % 4 == 0)
		{
			//SplineMesh->SetStaticMesh(RoadMeshL);
		}
		else
		{
			//SplineMesh->SetStaticMesh(RoadMeshNL);
		}
		/*
		FVector StartPos = PathSpline->GetLocationAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector StartTangent = PathSpline->GetDirectionAtDistanceAlongSpline(i*Spacing, ESplineCoordinateSpace::Local);
		FVector EndPosB = PathSpline->GetLocationAtDistanceAlongSpline((i+1)*Spacing, ESplineCoordinateSpace::Local);
		FVector EndTangentB = PathSpline->GetDirectionAtDistanceAlongSpline((i+1)*Spacing, ESplineCoordinateSpace::Local);
		float Diff = FVector(StartTangent - EndTangentB).Size();
		FVector EndPos = PathSpline->GetLocationAtDistanceAlongSpline((i + 1)*Spacing+Diff*Spacing/2, ESplineCoordinateSpace::Local);
		FVector EndTangent = PathSpline->GetDirectionAtDistanceAlongSpline((i + 1)*Spacing + Diff * Spacing / 2, ESplineCoordinateSpace::Local);
		SplineMesh->UpdateMesh();
		SplineMesh->RegisterComponent();
		SplineMesh->SetStartAndEnd(StartPos, StartTangent, EndPos, EndTangent);
		SplineMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SplineMesh->SetForwardAxis(ESplineMeshAxis::X);
		*/

		
		//TrackMeshes.AddUnique(SplineMesh);
	}
	
}
bool ATrackGenerator::BuildSpline_Validate()
{
	return true;
}

void ATrackGenerator::Generate_Implementation()
{
	
	for (USplineMeshComponent* SplineMesh : TrackMeshes)
	{
		SplineMesh->DestroyComponent();
	}
	TrackMeshes.Empty();
	TrackPoints.Empty();
	PathSpline->ClearSplinePoints();
	FTrackAlg::JoyInit(TrackPoints, SurroundingsHM, Ellipse_Width, Ellipse_Height, NumberOfPoints, StartingHeight, Steepness);
	bC = false;
}
bool ATrackGenerator::Generate_Validate()
{
	return true;
}

void ATrackGenerator::SetSteepness(float NewSteepness)
{
	Steepness = NewSteepness;
}

void ATrackGenerator::SetNewSpacing(float NewSpacing)
{
	Spacing = NewSpacing;
}

void ATrackGenerator::SetNumberOfPoints(int32 NewNumberOfPoints)
{
	NumberOfPoints = NewNumberOfPoints;
}

void ATrackGenerator::SetEllipseWidth(float NewEllipseWidth)
{
	Ellipse_Width = NewEllipseWidth;
}

void ATrackGenerator::SetEllpiseHeight(float NewEllipseHeight)
{
	Ellipse_Height = NewEllipseHeight;
}

float ATrackGenerator::GetSteepness() const
{
	return Steepness;
}

float ATrackGenerator::GetSpacing() const
{
	return Spacing;
}

int32 ATrackGenerator::GetNumberOfPoints() const
{
	return NumberOfPoints;
}

float ATrackGenerator::GetEllipseWidth() const
{
	return Ellipse_Width;;
}

float ATrackGenerator::GetEllipseHeight() const
{
	return Ellipse_Height;
}
void ATrackGenerator::SetupGS_Implementation()
{
	ATrackGS* TrackGS=Cast<ATrackGS>(UGameplayStatics::GetGameState(GetWorld()));
	if (TrackGS)
	{

		TrackGS->SetStartLocation(StartLocation);
		TrackGS->SetEndLocation(EndLocation);
		TrackGS->SetTrackSpline(PathSpline);
	}
	ATrackGM* TrackGM = Cast<ATrackGM>(UGameplayStatics::GetGameMode(GetWorld()));
	if (TrackGM)
	{
		TrackGM->SetupHUD();
		TrackGM->SetupStartingPoint();
		TrackGM->SpawnFinishLine();
	}

}
bool ATrackGenerator::SetupGS_Validate()
{
	return true;
}
void ATrackGenerator::GetLifetimeReplicatedProps(TArray< FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrackGenerator, TrackPoints);
	DOREPLIFETIME(ATrackGenerator, bC);
	DOREPLIFETIME(ATrackGenerator, TrackMeshes);
	DOREPLIFETIME(ATrackGenerator, RoadMeshL);
	DOREPLIFETIME(ATrackGenerator, RoadMeshNL);
	DOREPLIFETIME(ATrackGenerator, EndRoadMesh);
	DOREPLIFETIME(ATrackGenerator, Steepness);
	DOREPLIFETIME(ATrackGenerator, Spacing);
	DOREPLIFETIME(ATrackGenerator, TangentScale);
	DOREPLIFETIME(ATrackGenerator, bAutoSpacing);
	DOREPLIFETIME(ATrackGenerator, StartingHeight);
	DOREPLIFETIME(ATrackGenerator, NumberOfPoints);
	DOREPLIFETIME(ATrackGenerator, Ellipse_Width);
	DOREPLIFETIME(ATrackGenerator, Ellipse_Height);
	DOREPLIFETIME(ATrackGenerator, RoadScale);

}
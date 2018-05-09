// Fill out your copyright notice in the Description page of Project Settings.

#include "TrackAlg.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"


FTrackAlg* FTrackAlg::Runnable = NULL;

FTrackAlg::FTrackAlg(TArray<FVector>& NewTrack,TArray<TArray<TArray<FVector> >>& Surroundings, float NewEllipse_Width, float NewEllipse_Height, float NewNumberOfPoints, float NewStartingHeight, float NewSteepness)
{
	Track = &NewTrack;
	SurroundingsHM = &Surroundings;
	Ellipse_Width = NewEllipse_Width;
	Ellipse_Height = NewEllipse_Height;
	NumberOfPoints = NewNumberOfPoints;
	SStartingHeight = NewStartingHeight;
	Steepness = NewSteepness;
	Thread = FRunnableThread::Create(this, TEXT("FTrackAlg"), 0, TPri_AboveNormal);
}

FTrackAlg::~FTrackAlg()
{
	delete Thread;
	Thread = nullptr;
}

FVector2D FTrackAlg::GetRandomPointInEllipse()
{
	float t = 2.0f*PI*FMath::FRand();
	float u = FMath::FRand() + FMath::FRand();
	float r;
	if (u > 1.0f)
	{
		r = 2.0f - u;
	}
	else
	{
		r = u;
	}
	return FVector2D(Ellipse_Width*r*FMath::Cos(t) / 2.0f, Ellipse_Height*r*FMath::Sin(t) / 2.0f);
}

void FTrackAlg::GenerateHeightMap()
{

}

bool FTrackAlg::GetAngleBetweenTwoVectors(FVector A, FVector B)
{
	A.Normalize();
	B.Normalize();
	float Angle= FMath::RadiansToDegrees(FGenericPlatformMath::Acos(FVector::DotProduct(A, B)));
	if (Angle >= -50.0f&&Angle <= 50.0f)
		return true;
	else
		return false;
}

bool FTrackAlg::DoOverExtend(FVector A, FVector B)
{
	float Limit = 9000.0f;
	if (FMath::Abs(A.X - B.X) < Limit)
		return true;
	if (FMath::Abs(A.Y - B.Y) < Limit)
		return true;
	//if (FMath::Abs(A.X - B.Z) < Limit)
	//	return true;
	return false;
}

bool FTrackAlg::DoTracksOverlap(FEdge A)
{
	for (int32 i = 0; i < TrackTemp.Num() - 1; ++i)
	{
		FBox Box1(TrackTemp[i]-FVector(600.0f, 600.0f, 600.0f), TrackTemp[i + 1]+ FVector(600.0f, 600.0f, 600.0f));
		FBox Box2(A.Vertex[0]- FVector(600.0f, 600.0f, 600.0f), A.Vertex[1]+ FVector(600.0f, 600.0f, 600.0f));
		if (Box1.Overlap(Box2).GetSize() != FVector(0.0f, 0.0f, 0.0f))
		{
			return true;
		}
	}
	return false;
}

void FTrackAlg::CreateTrack()
{
	Voronoi VoronoiAlg;
	VoronoiEdges = VoronoiAlg.GetEdges(GeneratePoints(), Ellipse_Width, Ellipse_Width);
	/*
	Temp.Sort([this](const FVector2D& A, const FVector2D& B)
	{
		return A.X < B.X;
	});
	*/
	TrackTemp.Add(FVector(0.0f, 0.0f, SStartingHeight));
	TrackTemp.Add(FVector(0.0f, 0.0f, SStartingHeight));
	TrackTemp.Add(FVector(0.0f, -9000.0f, SStartingHeight));
	TrackTemp.Add(FVector(0.0f, -9000.0f, SStartingHeight));

	SStartingHeight *= Steepness;
	TArray<FEdge> Temp;
	for (int32 i = 0; i < VoronoiEdges.Num(); ++i)
	{

		//GetAngleBetweenTwoVectors(FVector(FVector2D(TrackTemp.Top()), 0.0f), VoronoiEdges[i].Vertex[1]) &&
	//	if (i == 0)
	//	{
		//	TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[0]), SStartingHeight));
		//	TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[1]), SStartingHeight));
		//	SStartingHeight *= Steepness;
		//	}
			UE_LOG(LogTemp, Warning, TEXT("Angle %f"), GetAngleBetweenTwoVectors(FVector(FVector2D(TrackTemp.Top()), 0.0f), VoronoiEdges[i].Vertex[0]));
			if (!DoOverExtend(TrackTemp.Top(), VoronoiEdges[i].Vertex[1]) && !DoTracksOverlap(VoronoiEdges[i]))
			{
				TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[0]), SStartingHeight));
				TrackTemp.AddUnique(FVector(FVector2D(VoronoiEdges[i].Vertex[1]), SStartingHeight));
				if(FMath::FRand()<0.5f)
				SStartingHeight *= Steepness;
			}


		

		
	}
/*
	for (int32 i = 0; i < Temp.Num(); ++i)
	{
		Track->Add(FVector(Temp[i], StartingHeight));

		
	}
*/
}

TArray<FVector2D> FTrackAlg::GeneratePoints()
{
	TArray<FVector2D> Generated;
	for (int32 i = 0; i < NumberOfPoints; ++i)
	{
		//Generated.Add(GetRandomPointInEllipse());
		Generated.Add(FVector2D(FMath::FRand()*Ellipse_Height, FMath::FRand()*Ellipse_Height));
	}
	return Generated;
}

bool FTrackAlg::Init()
{
	double Secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 Seed = (int32)(((int64)Secs) % INT_MAX);
	FMath::RandInit(Seed);
	return true;
}

uint32 FTrackAlg::Run()
{
	FPlatformProcess::Sleep(0.03);
	CreateTrack();

	for (int32 i=0;i<TrackTemp.Num();i+=2)
	{
		Track->Add(TrackTemp[i]);
	}

	bFinished = true;
	return 0;
}

void FTrackAlg::Stop()
{
	StopTaskCounter.Increment();
}

void FTrackAlg::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

FTrackAlg * FTrackAlg::JoyInit(TArray<FVector>& NewTrack, TArray<TArray<TArray<FVector> >>& Surroundings, float NewEllipse_Width, float NewEllipse_Height, float NewNumberOfPoints, float NewStartingHeight, float NewSteepness)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FTrackAlg(NewTrack,Surroundings, NewEllipse_Width, NewEllipse_Height, NewNumberOfPoints, NewStartingHeight, NewSteepness);
	}
	return Runnable;
}

void FTrackAlg::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FTrackAlg::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

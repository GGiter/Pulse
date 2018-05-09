// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/Surroundings/SpaceSphereAlg.h"
#include "Runtime/Core/Public/HAL/RunnableThread.h"
#include "Kismet/GameplayStatics.h"
#include "Public/Utilitys/PerlinNoise.h"

FSpaceSphereAlg* FSpaceSphereAlg::Runnable = NULL;

FSpaceSphereAlg::FSpaceSphereAlg(uint8* NewTextureColors, uint32 NewmData,int NewHeight,int NewWidth,float StarsDensity,float StarsBrightness,FVector2D NebulaScaleRange)
{
	TextureColors = NewTextureColors;
	mData = NewmData;
	Height = NewHeight;
	Width = NewWidth;
	NSRange = NebulaScaleRange;
	SDensity = StarsDensity;
	SBrightness = StarsBrightness;
	Thread = FRunnableThread::Create(this, TEXT("FSpaceSphereAlg"), 0, TPri_AboveNormal);
}

FSpaceSphereAlg::~FSpaceSphereAlg()
{
	delete Thread;
	Thread = nullptr;
}

void FSpaceSphereAlg::GeneratePointStars(float Density,float Brightness)
{	
	if (TextureColors)
	{
		int PixelAmount = mData / 4;
		int Count = FMath::FloorToInt(PixelAmount*Density);
		for (int i = 0; i < Count; ++i)
		{
			int R = FMath::FloorToInt(PixelAmount*FMath::FRand());
			int C = FMath::RoundToInt(255 * FMath::Loge(1 - FMath::FRand())*-Brightness);
			TextureColors[R * 4] = C;
			TextureColors[R * 4 + 1] = C;
			TextureColors[R * 4 + 2] = C;
		}
	}
	

}

float FSpaceSphereAlg::NebulaNoise(FVector2D P,PerlinNoise& PN)
{
	
	const int Steps = 5;
	float Scale = FMath::Pow(2.0f, float(Steps));
	float Displace = 0.0f;
	P += NOffset;
	for (int i = 0; i < Steps; ++i)
	{
		Displace = PN.Noise(P.X*Scale + Displace, P.Y*Scale + Displace,0.0f);
		Scale *= 0.5f;
	}
	return PN.Noise(P.X + Displace, P.Y+Displace,0.0f);
}

uint8* FSpaceSphereAlg::GenerateNebulas(float Density, float Falloff,float Scale)
{
	float n;
	uint8 * NewColors = TextureColors;
	FLinearColor OldColor,NewColor;
	PerlinNoise PN;
	if (TextureColors)
	{
		int PixelAmount = mData / 4;
		FLinearColor NebulaColor = FLinearColor::MakeRandomColor();
		for (int i = 0; i < PixelAmount; ++i)
		{
			//NebulaColor = FLinearColor::MakeRandomColor();
			n = NebulaNoise(FVector2D(i%Width,i/Width)*Scale,PN);
			
			n = FMath::Pow(n + Density, Falloff);
			OldColor = FLinearColor(NewColors[i * 4+2]/255.0f, NewColors[i * 4 + 1]/255.0f, NewColors[i * 4 ] / 255.0f);
			NewColor = OldColor + n * (NebulaColor - OldColor);
			NewColors[i * 4] = uint8(NewColor.B*255);
			NewColors[i * 4 + 1] = uint8(NewColor.G * 255);
			NewColors[i * 4 + 2] = uint8(NewColor.R * 255);
			NewColors[i * 4 + 3] = 110;
			
			
		}
		
	}
	return NewColors;
	
}



bool FSpaceSphereAlg::Init()
{
	double Secs = FTimespan(FDateTime::Now().GetTicks()).GetTotalSeconds();
	int32 Seed = (int32)(((int64)Secs) % INT_MAX);
	FMath::RandInit(Seed);
	return true;
}

uint32 FSpaceSphereAlg::Run()
{
	FPlatformProcess::Sleep(0.02);
	
	
	TextureColors=GenerateNebulas(FMath::FRandRange(0.0f,0.2f), FMath::FRandRange(3.0, 5.0f), FMath::FRandRange(NSRange.X, NSRange.Y));
	
	GeneratePointStars(SDensity, SBrightness);
	bFinished = true;
	return 0;
}

void FSpaceSphereAlg::Stop()
{
	StopTaskCounter.Increment();
}

void FSpaceSphereAlg::EnsureCompletion()
{
	Stop();
	Thread->WaitForCompletion();
}

void FSpaceSphereAlg::Shutdown()
{
	if (Runnable)
	{
		Runnable->EnsureCompletion();
		delete Runnable;
		Runnable = nullptr;
	}
}

bool FSpaceSphereAlg::IsThreadFinished()
{
	if (Runnable) return Runnable->IsFinished();
	return true;
}

FSpaceSphereAlg * FSpaceSphereAlg::JoyInit(uint8* NewTextureColors, uint32 NewmData, int NewHeight, int NewWidth, float StarsDensity, float StarsBrightness,FVector2D NebulaScaleRange)
{
	if (!Runnable && FPlatformProcess::SupportsMultithreading())
	{
		Runnable = new FSpaceSphereAlg(NewTextureColors,NewmData,NewHeight,NewWidth,StarsDensity,StarsBrightness,NebulaScaleRange);
	}
	return Runnable;
}

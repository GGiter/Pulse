// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Runtime/Core/Public/HAL/ThreadSafeCounter.h"
#include "Runtime/Core/Public/HAL/Runnable.h"
/**
 * 
 */
class PULSE_API FSpaceSphereAlg : public FRunnable
{
public:
	static FSpaceSphereAlg* Runnable;

	FRunnableThread* Thread;

	FThreadSafeCounter StopTaskCounter;
	bool IsFinished() const
	{
		return bFinished;
	}
	virtual bool Init() override;
	virtual uint32 Run() override;
	virtual void Stop() override;

	void EnsureCompletion();

	static void Shutdown();

	static bool IsThreadFinished();
	static FSpaceSphereAlg* JoyInit(uint8* NewTextureColors, uint32 NewmData, int NewHeight, int NewWidth, float StarsDensity, float StarsBrightness,FVector2D NebulaScaleRange);
	FSpaceSphereAlg(uint8* NewTextureColors, uint32 NewmData, int NewHeight, int NewWidth, float StarsDensity, float StarsBrightness, FVector2D NebulaScaleRange);
	~FSpaceSphereAlg();
private:
	void GeneratePointStars(float Density, float Brightness);
	float NebulaNoise(FVector2D P, class PerlinNoise& PN);
	uint8* GenerateNebulas(float Density, float Falloff,float Scale);
	bool bFinished = false;
	uint32 mData;
	uint8* TextureColors;
	int Height, Width;
	float SDensity, SBrightness;
	FVector2D NSRange;
	FVector2D NOffset = { 100.0f,100.0f };


};

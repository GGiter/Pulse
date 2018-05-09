// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PULSE_API PerlinNoise
{
public:
	PerlinNoise();
	PerlinNoise(uint32 Seed);
	~PerlinNoise();
	float Noise(float X, float Y, float Z);
private:
	// Permutation array
	TArray<int32> P;
	float Fade(float T);
	float Lerp(float T, float A, float B);
	float Grad(int32 Hash, float X, float Y, float Z);
};

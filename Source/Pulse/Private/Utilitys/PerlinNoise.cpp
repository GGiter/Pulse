// Fill out your copyright notice in the Description page of Project Settings.

#include "PerlinNoise.h"

PerlinNoise::PerlinNoise()
{
	P = {
		151,160,137,91,90,15,131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,
		8,99,37,240,21,10,23,190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,
		35,11,32,57,177,33,88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,
		134,139,48,27,166,77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,
		55,46,245,40,244,102,143,54, 65,25,63,161,1,216,80,73,209,76,132,187,208, 89,
		18,169,200,196,135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,
		250,124,123,5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,
		189,28,42,223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167,
		43,172,9,129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,
		97,228,251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,
		107,49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
		138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180 };

	TArray<int32> Temp = P;
	P.Append(Temp);
}

PerlinNoise::PerlinNoise(uint32 Seed)
{
	for (int32 i = 0; i < 256; ++i)
		P.Add(i);
	FMath::RandInit(Seed);
	P.Sort([this](const int32 Item1, const int32 Item2) {
		return FMath::FRand() < 0.5f;
	});
	TArray<int32> Temp = P;
	P.Append(Temp);
}

PerlinNoise::~PerlinNoise()
{
}

float PerlinNoise::Noise(float X, float Y, float Z)
{
	int32 x = (int32)floor(X) & 255;
	int32 y = (int32)floor(Y) & 255;
	int32 z = (int32)floor(Z) & 255;

	X -= floor(X);
	Y -= floor(Y);
	Z -= floor(Z);

	float u = Fade(X);
	float v = Fade(Y);
	float w = Fade(Z);

	int A = P[x] + y;
	int AA = P[A] + z;
	int AB = P[A + 1] + z;
	int B = P[x + 1] + y;
	int BA = P[B] + z;
	int BB = P[B + 1] + z;

	float Res = Lerp(w, Lerp(v, Lerp(u, Grad(P[AA], X, Y, Z), Grad(P[BA], X - 1, Y, Z)), Lerp(u, Grad(P[AB], X, Y - 1, Z), Grad(P[BB], X - 1, Y - 1, Z))), Lerp(v, Lerp(u, Grad(P[AA + 1], X, Y, Z - 1), Grad(P[BA + 1], X - 1, Y, Z - 1)), Lerp(u, Grad(P[AB + 1], X, Y - 1, Z - 1), Grad(P[BB + 1], X - 1, Y - 1, Z - 1))));
	return (Res + 1.0f) / 2.0f;
}
float PerlinNoise::Fade(float T)
{
	return T * T * T * (T * (T * 6 - 15) + 10);
}

float PerlinNoise::Lerp(float T, float A, float B)
{
	return A + T * (B - A);
}

float PerlinNoise::Grad(int32 Hash, float X, float Y, float Z)
{
	int h = Hash & 15;
	// Convert lower 4 bits of hash into 12 gradient directions
	double u = h < 8 ? X : Y,
		v = h < 4 ? Y : h == 12 || h == 14 ? X : Z;
	return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
}

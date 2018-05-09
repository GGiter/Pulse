// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VoronoiStructs.h"
/**
 * 
 */


class Voronoi
{
public:
	Voronoi();
	TArray<FEdge> GetEdges(TArray<FVector2D> NewVertices, float NewWidth, float NewHeight);
	~Voronoi();
private:
	void InsertParabola(FVector2D* Point);
	void RemoveParabola(FVoronoiEvent* Event);
	void FinishEdge(FParabola* Parabola);
	float GetXOfEdge(FParabola* Parabola, float Y);
	FParabola* GetParabolaByX(float XX);
	float GetY(FVector2D* Point, float X);
	void CheckCircle(FParabola *Parabola);
	FVector2D* GetEdgeIntersection(FVoronoiEdge* A, FVoronoiEdge* B);

	TArray<FVector2D> Places;
	TArray<FVoronoiEdge*> VoronoiEdges;
	float Width, Height;
	FParabola* Root;
	float CurrentY;

	TSet<FVoronoiEvent*> Deleted;
	TArray<FVector2D*> PointsList;
	TArray<FVoronoiEvent*> Queue;
	
};

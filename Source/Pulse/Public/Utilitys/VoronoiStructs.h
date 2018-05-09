// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */

struct FVoronoiEdge
{
public:
	FVector2D* Start;
	FVector2D* End;
	FVector2D* Direction;
	FVector2D* Left;
	FVector2D* Right;
	float F, G;
	FVoronoiEdge* Neighbour;
	FVoronoiEdge(FVector2D* NewStart, FVector2D* NewLeft, FVector2D* NewRight)
		: Start(NewStart), Left(NewLeft), Right(NewRight), End(nullptr), Neighbour(nullptr)
	{
		F = (Right->X - Left->X) / (Left->Y - Right->Y);
		G = Start->Y - F * Start->X;
		Direction = new FVector2D(Right->Y - Left->Y, -(Right->X - Left->X));
	}
};
struct FParabola
{
public:
	bool isLeaf;
	FVector2D* Site;
	FVoronoiEdge* Edge;
	struct FVoronoiEvent* VoronoiEvent;
	FParabola* Parent;
	FParabola()
		: Site(nullptr), isLeaf(false), VoronoiEvent(nullptr), Edge(nullptr), Parent(nullptr)
	{}
	FParabola(FVector2D* NewSite)
		: Site(NewSite), isLeaf(true), VoronoiEvent(nullptr), Edge(nullptr), Parent(nullptr)
	{}
	void SetLeft(FParabola* Parabola) { left = Parabola; Parabola->Parent = this; }
	void SetRight(FParabola* Parabola) { right = Parabola; Parabola->Parent = this; }

	FParabola*	Left() { return left; }
	FParabola* Right() { return right; }

	FParabola* left=nullptr;
	FParabola* right=nullptr;
	static FParabola* GetLeft(FParabola* Parabola);
	static FParabola* GetRight(FParabola* Parabola);
	static FParabola* GetLeftParent(FParabola* Parabola);
	static FParabola* GetRightParent(FParabola* Parabola);
	static FParabola* GetLeftChild(FParabola* Parabola);
	static FParabola* GetRightChild(FParabola* Parabola);
};

struct FVoronoiEvent
{
public:
	FVoronoiEvent(FVector2D* NewPoint, bool NewisPlaceEvent)
		: Point(NewPoint), isPlaceEvent(NewisPlaceEvent), Arch(nullptr)
	{
	}
	FVector2D* Point;
	bool isPlaceEvent;
	FParabola* Arch;
};
struct FVoronoiEventPredicate
{
	bool operator() (const FVoronoiEvent& A, const FVoronoiEvent& B) const
	{
		return A.Point->Y > B.Point->Y;
	}
};
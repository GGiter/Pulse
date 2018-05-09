// Fill out your copyright notice in the Description page of Project Settings.

#include "VoronoiStructs.h"

FParabola * FParabola::GetLeft(FParabola * Parabola)
{
	if (!Parabola)
		return nullptr;
	return GetLeftChild(GetLeftParent(Parabola));
}

FParabola * FParabola::GetRight(FParabola * Parabola)
{
	if (!Parabola)
		return nullptr;
	return GetRightChild(GetRightParent(Parabola));
}

FParabola * FParabola::GetLeftParent(FParabola * Parabola)
{
	if (!Parabola||!Parabola->Parent)
	{
		return nullptr;
	}
	FParabola* PParent = Parabola->Parent;
	FParabola* LastParabola = Parabola;
	
	while (PParent->Left() == LastParabola)
	{
		if (!PParent->Parent)
		{
			return nullptr;
		}
		LastParabola = PParent;
		PParent = PParent->Parent;
	}
	
	return PParent;
}

FParabola * FParabola::GetRightParent(FParabola * Parabola)
{
	if (!Parabola || !Parabola->Parent)
	{
		return nullptr;
	}
	FParabola* PParent = Parabola->Parent;
	FParabola* LastParabola = Parabola;
	
	while (PParent->Right() == LastParabola)
	{
		if (!PParent->Parent)
		{
			return nullptr;
		}
		LastParabola = PParent;
		PParent = PParent->Parent;
	}
	return PParent;
}

FParabola * FParabola::GetLeftChild(FParabola* Parabola)
{
	if (!Parabola || Parabola->Left())
	{
		return nullptr;
	}
	FParabola* LeftParabola = Parabola->Left();
	while (!LeftParabola->isLeaf)
	{
		if (!LeftParabola->Right())
			return nullptr;
		LeftParabola = LeftParabola->Right();
	}
	return Parabola;
}

FParabola * FParabola::GetRightChild(FParabola * Parabola)
{
	if (!Parabola|| Parabola->Right())
	{
		return nullptr;
	}
	FParabola* RightParabola = Parabola->Right();
	while (!RightParabola->isLeaf)
	{
		if (!RightParabola->Left())
			return nullptr;
		RightParabola = RightParabola->Left();
	}
	return RightParabola;
}
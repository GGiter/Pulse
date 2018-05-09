// Fill out your copyright notice in the Description page of Project Settings.

#include "Voronoi.h"





Voronoi::Voronoi()
{

}
Voronoi::~Voronoi()
{
}

TArray<FEdge> Voronoi::GetEdges(TArray<FVector2D> NewVertices, float NewWidth, float NewHeight)
{
	Places = NewVertices;
	Width = NewWidth;
	Height = NewHeight;
	Root = nullptr;
	PointsList.Empty();
	VoronoiEdges.Empty();
	
	for (FVector2D Place : Places)
	{
		Queue.HeapPush(new FVoronoiEvent(new FVector2D(Place), true), FVoronoiEventPredicate());
	}
	Queue.Heapify(FVoronoiEventPredicate());
	
	FVoronoiEvent* E;
	while (Queue.Num())
	{
		E = Queue.HeapTop();
		Queue.HeapPop(E,FVoronoiEventPredicate());
		CurrentY = E->Point->Y;
		if (Deleted.Find(E) != nullptr)
		{
			
			delete(E);
			Deleted.Remove(E);
			continue;
		}
		else if (E->isPlaceEvent)
		{
			InsertParabola(E->Point);
			
		}
		else
		{
			RemoveParabola(E);
		}
		delete(E);
	}
	if(Root != nullptr)
	FinishEdge(Root);
	for (FVoronoiEdge* Edge : VoronoiEdges)
	{
		if (Edge->Neighbour)
		{
			Edge->Start = Edge->Neighbour->End;
			delete Edge->Neighbour;
		}
	}
	TArray<FEdge> Edges;
	for (FVoronoiEdge* Edge : VoronoiEdges)
	{
		if(Edge->Start->Y<Edge->End->Y)
		Edges.Add(FEdge(FVector(*Edge->Start, 0.0f), FVector(*Edge->End, 0.0f)));
		else
		Edges.Add(FEdge(FVector(*Edge->End, 0.0f), FVector(*Edge->Start, 0.0f)));
	}
	return Edges;
}



void Voronoi::InsertParabola(FVector2D* Point)
{
	if (Root == nullptr)
	{
		Root = new FParabola(Point);
		return;
	}
	
	if (Root->isLeaf && Root->Site->Y - Point->Y < 1.0f)
	{
		FVector2D* FPoint = Root->Site;
		Root->isLeaf = false;
		Root->SetLeft(new FParabola(FPoint));
		Root->SetRight(new FParabola(Point));
		FVector2D* S= new FVector2D((Point->X + FPoint->X) / 2, Height);
		PointsList.Push(S);
		if (Point->X > FPoint->X)
		{
			Root->Edge = new FVoronoiEdge(S, FPoint, Point);
		}
		else
		{
			Root->Edge = new FVoronoiEdge(S, Point, FPoint);
		}
		VoronoiEdges.Add(Root->Edge);
		return;
	}

	FParabola* Parabola = GetParabolaByX(Point->X);
	if (!Parabola)
		return;
	if (Parabola->VoronoiEvent)
	{
		Deleted.Add(Parabola->VoronoiEvent);
		Parabola->VoronoiEvent = nullptr;
	}
	FVector2D* Start = new FVector2D(Point->X, GetY(Parabola->Site, Point->X));
	PointsList.Push(Start);

	FVoronoiEdge * EL = new FVoronoiEdge(Start, Parabola->Site, Point);
	FVoronoiEdge* ER = new FVoronoiEdge(Start, Point, Parabola->Site);

	EL->Neighbour = ER;
	VoronoiEdges.Add(EL);

	Parabola->Edge = ER;
	Parabola->isLeaf = false;

	FParabola* P0 = new FParabola(Parabola->Site);
	FParabola* P1 = new FParabola(Point);
	FParabola* P2 = new FParabola(Parabola->Site);

	Parabola->SetRight(P2);
	Parabola->SetLeft(new FParabola());
	Parabola->Left()->Edge = EL;

	Parabola->Left()->SetLeft(P0);
	Parabola->Left()->SetRight(P1);

	CheckCircle(P0);
	CheckCircle(P2);
}

void Voronoi::RemoveParabola(FVoronoiEvent * Event)
{
	FParabola* P1 = Event->Arch;

	FParabola* XL = FParabola::GetLeftParent(P1);
	FParabola* XR = FParabola::GetRightParent(P1);

	FParabola* P0 = FParabola::GetLeftChild(XL);
	FParabola* P2 = FParabola::GetRightChild(XR);

	if (P0->VoronoiEvent)
	{
		Deleted.Emplace(P0->VoronoiEvent);
		P0->VoronoiEvent = nullptr;
	}
	if (P2->VoronoiEvent)
	{
		Deleted.Emplace(P2->VoronoiEvent);
		P2->VoronoiEvent = nullptr;
	}
	FVector2D* Point = new FVector2D(Event->Point->X, GetY(P1->Site, Event->Point->X));
	PointsList.Push(Point);

	XL->Edge->End = Point;
	XR->Edge->End = Point;

	FParabola* Higher = nullptr;
	FParabola* Parabola = P1;
	while (Parabola != Root)
	{
		Parabola = Parabola->Parent;
		if (Parabola == XL) Higher = XL;
		if (Parabola == XR) Higher = XR;
	}
	if (Higher)
	{
		Higher->Edge = new FVoronoiEdge(Point, P0->Site, P2->Site);
	VoronoiEdges.Add(Higher->Edge);
	}
	

	FParabola * PParent = P1->Parent->Parent;
	if (P1->Parent->Left() == P1)
	{
		if (PParent->Left() == P1->Parent) PParent->SetLeft(P1->Parent->Right());
		if (PParent->Right() == P1->Parent) PParent->SetRight(P1->Parent->Right());
	}
	else
	{
		if (PParent->Left() == P1->Parent) PParent->SetLeft(P1->Parent->Left());
		if (PParent->Right() == P1->Parent) PParent->SetRight(P1->Parent->Left());
	}

	delete P1->Parent;
	delete P1;
	CheckCircle(P0);
	CheckCircle(P2);
}

void Voronoi::FinishEdge(FParabola * Parabola)
{
	if (Parabola->isLeaf)
	{
		delete Parabola;
		return;
	}
	float mx;
	if (Parabola->Edge->Direction->X > 0.0f)
	{
		mx = FMath::Max(Width, Parabola->Edge->Start->X + 10.0f);
	}
	else
	{
		mx = FMath::Min(0.0f, Parabola->Edge->Start->X + 10.0f);
	}

	FVector2D* End = new FVector2D(mx, mx*Parabola->Edge->F + Parabola->Edge->G);
	Parabola->Edge->End = End;
	PointsList.Push(End);
	FinishEdge(Parabola->Left());
	FinishEdge(Parabola->Right());
	delete Parabola;
}

float Voronoi::GetXOfEdge(FParabola * Parabola, float Y)
{
	FParabola * Left = FParabola::GetLeftChild(Parabola);
	FParabola * Right = FParabola::GetRightChild(Parabola);
	if (!Left || !Right || !Parabola)
		return 0.0f;
	FVector2D* P = Left->Site;
	FVector2D* R = Right->Site;

	float DP = 2.0 * (P->Y - Y);
	float A1 = 1.0 / DP;
	float B1 = -2.0 * P->X / DP;
	float C1 = Y + DP / 4 + P->X * P->X / DP;

	DP = 2.0 * (R->Y - Y);
	float A2 = 1.0 / DP;
	float B2 = -2.0 * R->X / DP;
	float C2 = CurrentY + DP / 4 + R->X * R->X / DP;

	float a = A1 - A2;
	float b = B1 - B2;
	float c = C1 - C2;

	float disc = b * b - 4 * a * c;
	float X1 = (-b + FMath::Sqrt(disc)) / (2 * a);
	float X2 = (-b - FMath::Sqrt(disc)) / (2 * a);

	float ry;
	if (P->Y < R->Y) ry = FMath::Max(X1, X2);
	else ry = FMath::Min(X1, X2);

	return ry;
}

FParabola * Voronoi::GetParabolaByX(float XX)
{
	FParabola* Parabola = Root;
	float X=0.0f;
	while (Parabola->isLeaf == false)
	{
		if (!Parabola)
			return nullptr;

		X = GetXOfEdge(Parabola, CurrentY);
		if (X > XX)
		{
			Parabola = Parabola->Left();
		}
		else
		{
			Parabola = Parabola->Right();
		}
		

	}
	return Parabola;
}

float Voronoi::GetY(FVector2D * Point, float X)
{
	float DP = 2 * (Point->Y - CurrentY);
	float A1 = 1 / DP;
	float B1 = -2 * Point->X / DP;
	float C1 = CurrentY + DP / 4 + Point->X * Point->X / DP;

	return(A1*X*X + B1 * X + C1);
}

void Voronoi::CheckCircle(FParabola * Parabola)
{
	FParabola * LParabola = FParabola::GetLeftParent(Parabola);
	FParabola * RParabola = FParabola::GetRightParent(Parabola);

	FParabola * A = FParabola::GetLeftChild(LParabola);
	FParabola * C = FParabola::GetRightChild(RParabola);

	if (!A || !C || A->Site == C->Site) return;

	FVector2D* S=nullptr;
	S = GetEdgeIntersection(LParabola->Edge, RParabola->Edge);
	if (S == nullptr) return;

	float dx = A->Site->X - S->X;
	float dy = A->Site->Y - S->Y;

	float d = FMath::Sqrt((dx * dx) + (dy * dy));

	if (S->Y - d >= CurrentY) { return; }

	FVoronoiEvent * E = new FVoronoiEvent(new FVector2D(S->X, S->Y - d), false);
	PointsList.Push(E->Point);
	Parabola->VoronoiEvent = E;
	E->Arch = Parabola;
	Queue.HeapPush(E, FVoronoiEventPredicate());
}

FVector2D* Voronoi::GetEdgeIntersection(FVoronoiEdge* A, FVoronoiEdge* B)
{
	float X = (B->G - A->G) / (A->F - B->F);
	float Y = (A->F * X + A->G);

	if ((X - A->Start->X) / A->Direction->X < 0) return nullptr;
	if ((Y - A->Start->Y) / A->Direction->Y < 0) return nullptr;

	if ((X - B->Start->X) / B->Direction->X < 0) return nullptr;
	if ((Y - B->Start->Y) / B->Direction->Y < 0) return nullptr;

	FVector2D* Point=new FVector2D(X, Y);
	PointsList.Push(Point);
	return Point;
}

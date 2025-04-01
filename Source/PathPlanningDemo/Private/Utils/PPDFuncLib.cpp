// Fill out your copyright notice in the Description page of Project Settings.


#include "Utils/PPDFuncLib.h"

float UPPDFuncLib::GetDistanceBetweenPoints(const FVector2D& PointA, const FVector2D& PointB,
	const EDistanceType DistanceType)
{
	float Result = 0.0f;
	switch (DistanceType)
	{
	case EDistanceType::EDT_ManhattanDistance:
		Result = FMath::Abs(PointA.X - PointB.X) + FMath::Abs(PointA.Y - PointB.Y);
		break;

	case EDistanceType::EDT_ChebyshevDistance:
		Result = FMath::Max(FMath::Abs(PointA.X - PointB.X),FMath::Abs(PointA.Y - PointB.Y));
		break;

	case EDistanceType::EDT_EuclideanDistance:
		Result = FVector2D::Distance(PointA,PointB);
		break;

	case EDistanceType::Max:
	default:
		break;
	}
	return Result;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPDBaseStructures.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "PPDFuncLib.generated.h"

/**
 * 
 */
UCLASS()
class PATHPLANNINGDEMO_API UPPDFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static float GetDistanceBetweenPoints(
		const FVector2D& PointA,
		const FVector2D& PointB,
		const EDistanceType DistanceType = EDistanceType::EDT_ManhattanDistance);
};

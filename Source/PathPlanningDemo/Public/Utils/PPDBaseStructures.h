// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPDBaseStructures.generated.h"

/**
 *  Base Structures For Path Planning Demo
 */

class APPDCellBase;

//CellType
UENUM(BlueprintType)
enum class ECellType : uint8
{
	ECT_Empty = 0 ,
	ECT_Water = 1 ,
	ECT_Mountain = 2,
	ECT_Path = 3,
	ECT_Start = 4,
	ECT_Dest = 5,
	ECT_OpenList = 6,
	ECT_CloseList = 7,
	Max
};

UENUM(BlueprintType)
enum class EPathPlanningType : uint8
{
	EPPT_AStar = 0 ,
	EPPT_FlowField = 1 ,
	EPPT_JumpPointSearch = 2,
	Max
};

UENUM(BlueprintType)
enum class EDistanceType : uint8
{
	EDT_ManhattanDistance = 0 ,
	EDT_EuclideanDistance = 1 ,
	EDT_ChebyshevDistance = 2 ,
	Max
};

//################## Path Planning ##################

//AStar Node
struct FAStarNode
{
	FVector2D Location;
	ECellType CellType;
	float CostG;
	float CostH;
	TSharedPtr<FAStarNode, ESPMode::ThreadSafe> Parent;

	explicit FAStarNode():
	Location(FVector2D(0,0)),
	CellType(ECellType::ECT_Empty),
	CostG(0.0f),
	CostH(0.0f),
	Parent(nullptr){}

	explicit FAStarNode(const FVector2D InLocation,
		const ECellType InCellType,
		const float InCost_G,
		const float InCost_H,
		const TSharedPtr<FAStarNode, ESPMode::ThreadSafe>& InParent):
	Location(InLocation),
	CellType(InCellType),
	CostG(InCost_G),
	CostH(InCost_H),
	Parent(InParent){}

	float CostF() const { return CostG + CostH; }
};

//AStar Node Predicate
struct FAStarPredicate
{
	bool operator()(const TSharedPtr<FAStarNode>& A, const TSharedPtr<FAStarNode>& B) const
	{
		return A->CostF() < B->CostF();
	}
};

//################ Path Planning End #################
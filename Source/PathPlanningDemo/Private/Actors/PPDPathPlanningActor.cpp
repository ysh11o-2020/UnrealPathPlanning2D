// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PPDPathPlanningActor.h"
#include "Algo/Reverse.h"
#include "Utils/PPDFuncLib.h"

// Sets default values
APPDPathPlanningActor::APPDPathPlanningActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APPDPathPlanningActor::BeginPlay()
{
	Super::BeginPlay();
	
}

bool APPDPathPlanningActor::PathPlanning(const TMap<FVector2D, TObjectPtr<APPDCellBase>>& MapInfo,
	const EPathPlanningType PathPlanningType, const FVector2D Start, const FVector2D Destination,
	TArray<FVector2D>& OutPath)
{
	if (!MapInfo.Contains(Start) || !MapInfo.Contains(Destination))
	if (MapInfo.Contains(Start) && !MapInfo[Start].Get()->IsPassable())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, "Error: Start Point is not valid");
		}
		return false;
	}
	
	if (MapInfo.Contains(Destination) && !MapInfo[Destination].Get()->IsPassable())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(0, 3.0f, FColor::Red, "Error: Destination Point is not valid");
		}
		return false;
	}
	
	switch (PathPlanningType)
	{
	case EPathPlanningType::EPPT_AStar:
		return PathPlanningAStarCore(MapInfo, Start, Destination, OutPath);

	case EPathPlanningType::EPPT_FlowField:
	case EPathPlanningType::EPPT_JumpPointSearch:
	case EPathPlanningType::Max:
	default:
		return false;
	}
}

bool APPDPathPlanningActor::PathPlanningAStarCore(const TMap<FVector2D, TObjectPtr<APPDCellBase>>& MapInfo, const FVector2D Start, const FVector2D Destination, TArray<FVector2D>& OutPath)
{
	OutPath.Empty();

	TArray<TSharedPtr<FAStarNode>> OpenList;
	TSet<FVector2D> ClosedList;
	
	TSharedPtr<FAStarNode> StartNode = MakeShared<FAStarNode>(
		Start,
		/*start point default type is empty*/ECellType::ECT_Empty,
		0.0f,
		UPPDFuncLib::GetDistanceBetweenPoints(Start, Destination),
		nullptr);

	OpenList.HeapPush(StartNode,FAStarPredicate());

	int32 LoopCounter = 0;
	
	while (!OpenList.IsEmpty())
	{
		if (LoopCounter > MaxLoopCount)
		{
			UE_LOG(LogTemp, Error, TEXT("AStar Can't Find a valid path, Loop count exceeded"));
			return false;
		}
		
		LoopCounter++;
		TSharedPtr<FAStarNode> CurrentNode;
		OpenList.HeapPop(CurrentNode,FAStarPredicate());

		if (CurrentNode->Location == Destination)
		{
			TSharedPtr<FAStarNode> Node = CurrentNode;
			while (Node!=nullptr)
			{
				OutPath.Push(Node->Location);
				Node = Node->Parent;
			}
			Algo::Reverse(OutPath);

			if (GEngine)
			{
				FString DebugStr = FString::Printf(TEXT("AStar LoopCount[%d] Find a Valid Path(Length:[%d])"),LoopCounter,OutPath.Num());
				GEngine->AddOnScreenDebugMessage(0, 5.0f, FColor::Green, DebugStr);
			}
			
			return true;
		}

		ClosedList.Add(FVector2D(CurrentNode->Location.X, CurrentNode->Location.Y));
		
		for (int32 i=0;i<8;i++)
		{
			int32 NextX = CurrentNode->Location.X + DirX[i];
			int32 NextY = CurrentNode->Location.Y + DirY[i];
			FVector2D NextLocation(NextX, NextY);
			
			if (const TObjectPtr<APPDCellBase>* CellPtr = MapInfo.Find(NextLocation))
			{
				if ((*CellPtr)->IsPassable() && !ClosedList.Contains(NextLocation))
				{
					float DeltaCostG = FMath::Abs(DirX[i]+DirY[i]) != 1.0f ? 1.4f : 1.0f;
					float NewCostG = CurrentNode->CostG + DeltaCostG;

					ECellType NewCellType = MapInfo[NextLocation]->GetCellType();
				
					TSharedPtr<FAStarNode> NeighborNode = MakeShared<FAStarNode>(
						FVector2D(NextX, NextY),
						NewCellType,
						NewCostG,
						UPPDFuncLib::GetDistanceBetweenPoints(NextLocation, Destination,EDistanceType::EDT_ManhattanDistance),
						CurrentNode);

					// 在 HeapPush 前检查是否存在更优路径
					bool bFoundInOpenList = false;
					for (const TSharedPtr<FAStarNode>& Node : OpenList)
					{
						if (Node->Location == NextLocation)
						{
							bFoundInOpenList = true;
							if (NewCostG < Node->CostG)
							{
								Node->CostG = NewCostG;
								Node->Parent = CurrentNode;
								OpenList.Heapify(FAStarPredicate());
							}
							break;
						}
					}
					
					if (!bFoundInOpenList)
					{
						OpenList.HeapPush(NeighborNode, FAStarPredicate());
					}
					
				}
			}
		}
	}
	return false;
}

bool APPDPathPlanningActor::PathPlanningFlowFieldCore(const TMap<FVector2D, TObjectPtr<APPDCellBase>>& MapInfo, const FVector2D Start, const FVector2D Destination, TArray<FVector2D>& OutPath)
{
	return true;
}

bool APPDPathPlanningActor::PathPlanningJPSCore(const TMap<FVector2D, TObjectPtr<APPDCellBase>>& MapInfo, const FVector2D Start, const FVector2D Destination, TArray<FVector2D>& OutPath)
{
	return true;
}

// Called every frame
void APPDPathPlanningActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APPDPathPlanningActor::BPPathPlanning(APPDGameModeBase* PPDGameMode, TArray<FVector2D>& OutPath)
{
	return PathPlanning(
		PPDGameMode->GetMapInfo(),
		EPathPlanningType::EPPT_AStar,
		FVector2D(0,0),
		FVector2D(49,49),
		OutPath);
}


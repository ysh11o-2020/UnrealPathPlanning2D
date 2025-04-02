// Fill out your copyright notice in the Description page of Project Settings.


#include "Gameplay/PPDGameModeBase.h"

#include "GenericQuadTree.h"
#include "Math/UnitConversion.h"



APPDGameModeBase::APPDGameModeBase()
{
	MapInfo.Empty();
	EmptyCellPercent = 0.5f;
	WaterCellPercentInBlock = 0.5f;
	MapWidth = 100;
	MapHeight = 100;
}

void APPDGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}

TMap<FVector2D, TObjectPtr<APPDCellBase>> APPDGameModeBase::GetMapInfo() const
{
	return MapInfo;
}

void APPDGameModeBase::InitMap()
{
	MapInfo.Empty();
	checkf(CellBaseClass,TEXT("Please set your CellBaseClass!"));
	for (int32 i = 0; i < MapWidth; i++)
	{
		for (int32 j = 0; j < MapHeight; j++)
		{
			FVector Position = FVector(i*100.0f+50.0f, j*100.0f+50.0f, 50.0f);
			FActorSpawnParameters SpawnParams;
			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
			TObjectPtr<APPDCellBase> CellBaseActor = GetWorld()->SpawnActor<APPDCellBase>(CellBaseClass, Position,FRotator(0.0f), SpawnParams);
			if (CellBaseActor != nullptr)
			{
				CellBaseActor->SetLocation2D(FVector2D(i,j));
				const float RandomFloatValue = FMath::RandRange(0.0f,1.0f);
				if (RandomFloatValue < EmptyCellPercent)
				{
					CellBaseActor->SetCellType(ECellType::ECT_Empty);
				}
				else if (RandomFloatValue > EmptyCellPercent && RandomFloatValue < EmptyCellPercent + (1.0f - EmptyCellPercent) * WaterCellPercentInBlock)
				{
					CellBaseActor->SetCellType(ECellType::ECT_Water);
				}
				else
				{
					CellBaseActor->SetCellType(ECellType::ECT_Mountain);
				}
				MapInfo.Emplace(FVector2D(i,j),CellBaseActor);
			}
		}
	}
}

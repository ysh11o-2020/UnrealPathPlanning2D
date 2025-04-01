// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/PPDCellBase.h"
#include "Chaos/PBDSuspensionConstraintData.h"

// Sets default values
APPDCellBase::APPDCellBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CellType = ECellType::ECT_Empty;
	
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	RootComponent = MeshComponent;
}

void APPDCellBase::SetCellType(const ECellType InCellType)
{
	if (InCellType == ECellType::ECT_Path && CellType == ECellType::ECT_Mountain) return;
		
	CellType = InCellType;
	TObjectPtr<UStaticMesh> CellMesh = *CellTypeMap.Find(InCellType);
	if (CellMesh != nullptr)
	{
		MeshComponent->SetStaticMesh(CellMesh);
	}
}

FVector2D APPDCellBase::GetLocation2D() const
{
	return Location2D;
}

void APPDCellBase::SetLocation2D(const FVector2D InLocation2D)
{
	Location2D = InLocation2D;
}

bool APPDCellBase::IsPassable(const bool bCanSwim) const
{
	if (bCanSwim)
	{
		return CellType != ECellType::ECT_Mountain;
	}
	else
	{
		return CellType == ECellType::ECT_Empty;
	}
}

// Called when the game starts or when spawned
void APPDCellBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APPDCellBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


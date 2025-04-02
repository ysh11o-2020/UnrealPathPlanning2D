// Fill out your copyright notice in the Description page of Project Settings.


#include "QuadTree/QuadTree.h"

#include "Kismet/KismetMathLibrary.h"

AQuadTree::AQuadTree()
{
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AQuadTree::BeginPlay()
{
	Super::BeginPlay();
	QuadTreeNode::WorldObject = GetWorld();
	Root = MakeShareable(new QuadTreeNode(FVector::ZeroVector,FVector(Height,Width,0),0));
	GetWorld()->GetTimerManager().SetTimer(Timer_Spawn,this,&AQuadTree::SpawnActors,PlayRate,true);
	GetWorld()->GetTimerManager().SetTimer(Timer_Spawn,this,&AQuadTree::ActorsAddVelocity,2,true);
}

// Called every frame
void AQuadTree::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (Root.IsValid())
	{
		Root->UpdateState();	//更新状态
		Root->TraceObjectInRange(TraceActor,AffectRadiusRange);	//判断是否在扫描器的范围内
	}
}

void AQuadTree::SpawnActors()
{
	if (CubeCount < 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(Timer_Spawn);
		return;
	}
	CubeCount--;
	FVector Pos = FVector(
		UKismetMathLibrary::RandomIntegerInRange(-Height+10,Height-10),
		UKismetMathLibrary::RandomIntegerInRange(-Width+10,Width-10),
		11);

	FTransform Trans = FTransform(FRotator(0, UKismetMathLibrary::RandomFloatInRange(0, 360), 0), Pos, FVector(0.2));
	AActor* Battery = GetWorld()->SpawnActor<AActor>(BatteryClass, Trans);
	if (IsValid(Battery))
	{
		Objects.Add(Battery);
		Root->InsertObject(Battery);	
	}
}

void AQuadTree::ActorsAddVelocity()
{
	for (AActor* Battery :Objects)
	{
		Battery->GetComponentByClass<UStaticMeshComponent>()->SetPhysicsLinearVelocity(UKismetMathLibrary::RandomUnitVector() * 50);
	}
}


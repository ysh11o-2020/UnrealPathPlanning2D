// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PPDCellBase.h"
#include "GameFramework/Actor.h"
#include "Gameplay/PPDGameModeBase.h"
#include "Utils/PPDBaseStructures.h"
#include "PPDPathPlanningActor.generated.h"

UCLASS()
class PATHPLANNINGDEMO_API APPDPathPlanningActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPDPathPlanningActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	bool BPPathPlanning(APPDGameModeBase* PPDGameMode, TArray<FVector2D>& OutPath);
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	bool PathPlanning(const TMap<FVector2D,TObjectPtr<APPDCellBase>>& MapInfo,
		const EPathPlanningType PathPlanningType,
		const FVector2D Start,
		const FVector2D Destination,
		TArray<FVector2D>& OutPath);

private:
	//################################# PathPlanning2D Core Functions Begin #####################################

	const TArray<int32> DirX = {-1,1,0,0,-1,-1,1,1};
	const TArray<int32> DirY = {0,0,-1,1,-1,1,-1,1};

	const int32 MaxLoopCount = 1000000;
	
	bool PathPlanningAStarCore(const TMap<FVector2D,TObjectPtr<APPDCellBase>>& MapInfo,
		const FVector2D Start,
		const FVector2D Destination,
		TArray<FVector2D>& OutPath);
	
	bool PathPlanningFlowFieldCore(const TMap<FVector2D,TObjectPtr<APPDCellBase>>& MapInfo,
		const FVector2D Start,
		const FVector2D Destination,
		TArray<FVector2D>& OutPath);

	bool PathPlanningJPSCore(const TMap<FVector2D,TObjectPtr<APPDCellBase>>& MapInfo,
		const FVector2D Start,
		const FVector2D Destination,
		TArray<FVector2D>& OutPath);

	//################################# PathPlanning2D Core Functions End #####################################
};

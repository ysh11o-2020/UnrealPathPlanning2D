// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuadTreeNode.h"
#include "GameFramework/Actor.h"
#include "QuadTree.generated.h"

UCLASS()
class PATHPLANNINGDEMO_API AQuadTree : public AActor
{
	GENERATED_BODY()
	
public:	
	AQuadTree();
	
	virtual void Tick(float DeltaTime) override;

	void SpawnActors();
	void ActorsAddVelocity();

	UPROPERTY(EditAnywhere)
	int32 CubeCount = 20;

	UPROPERTY(EditAnywhere)
	int32 Width = 500;

	UPROPERTY(EditAnywhere)
	int32 Height = 500;

	UPROPERTY(EditAnywhere)
	float PlayRate = 0.05f;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> BatteryClass;

	UPROPERTY(EditAnywhere)
	AActor* TraceActor;

	UPROPERTY(EditAnywhere)
	float AffectRadiusRange = 50.0f;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> Objects;

	TSharedPtr<QuadTreeNode> Root;
	FTimerHandle Timer_Spawn;
	FTimerHandle Timer_AddVelocity;
	
protected:
	virtual void BeginPlay() override;
	
};

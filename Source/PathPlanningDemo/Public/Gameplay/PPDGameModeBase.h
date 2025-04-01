// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/PPDCellBase.h"
#include "GameFramework/GameModeBase.h"
#include "PPDGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class PATHPLANNINGDEMO_API APPDGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	APPDGameModeBase();
	
	virtual void BeginPlay() override;
	
	TMap<FVector2D,TObjectPtr<APPDCellBase>> GetMapInfo() const;
	
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APPDCellBase> CellBaseClass;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BaseSettings",meta=(ClampMin="0.0",ClampMax="1.0"))
	float EmptyCellPercent;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BaseSettings",meta=(ClampMin="0.0",ClampMax="1.0"))
	float WaterCellPercentInBlock;
	
	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BaseSettings",meta=(ClampMin="5",ClampMax="100"))
	int32 MapWidth;

	UPROPERTY(EditDefaultsOnly,BlueprintReadWrite,Category="BaseSettings",meta=(ClampMin="5",ClampMax="100"))
	int32 MapHeight;
	
	UFUNCTION(BlueprintCallable)
	void InitMap();

	UPROPERTY(BlueprintReadOnly)
	TMap<FVector2D,TObjectPtr<APPDCellBase>> MapInfo;

	
};

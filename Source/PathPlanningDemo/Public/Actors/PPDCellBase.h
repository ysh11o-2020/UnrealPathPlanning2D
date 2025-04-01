// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Utils/PPDBaseStructures.h"
#include "PPDCellBase.generated.h"

UCLASS()
class PATHPLANNINGDEMO_API APPDCellBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	APPDCellBase();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	ECellType GetCellType() const;

	UFUNCTION(BlueprintCallable)
	void SetCellType(const ECellType InCellType);

	UFUNCTION(BlueprintCallable)
	FVector2D GetLocation2D() const;

	UFUNCTION(BlueprintCallable)
	void SetLocation2D(const FVector2D InLocation2D);

	UFUNCTION(BlueprintCallable)
	bool IsPassable(const bool bCanSwim = true) const;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Components", meta=(AllowPrivateAccess=true))
	TObjectPtr<class UStaticMeshComponent> MeshComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="BaseSettings", meta=(AllowPrivateAccess=true))
	ECellType CellType;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseSettings", meta=(AllowPrivateAccess=true))
	TMap<ECellType, TObjectPtr<UStaticMesh>> CellTypeMap;

private:
	FVector2D Location2D;
};

inline ECellType APPDCellBase::GetCellType() const
{
	return CellType;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Battery.generated.h"

UCLASS()
class PATHPLANNINGDEMO_API ABattery : public AActor
{
	GENERATED_BODY()
	
public:	
	ABattery();

	virtual void Tick(float DeltaTime) override;
	void ActivateState(bool bInActive, AActor* Target);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> NormalMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterial> ActiveMaterial;

	UPROPERTY(EditAnywhere)
	TObjectPtr<AActor> TargetActor;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UStaticMeshComponent> StaticMeshComponent;

	bool bActive = false;

protected:
	virtual void BeginPlay() override;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Battery.h"

#include "Kismet/KismetSystemLibrary.h"

ABattery::ABattery()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	SetRootComponent(StaticMeshComponent);

	StaticMeshComponent->SetMobility(EComponentMobility::Movable);
	StaticMeshComponent->SetConstraintMode(EDOFMode::XYPlane);
	StaticMeshComponent->SetSimulatePhysics(true);
}

void ABattery::BeginPlay()
{
	Super::BeginPlay();
}

void ABattery::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bActive && TargetActor)
	{
		float DrawTime = 1 / UKismetSystemLibrary::GetFrameCount();
		DrawDebugLine(GetWorld(),GetActorLocation(),TargetActor->GetActorLocation(),FColor::Cyan,false,DrawTime,1,4.0f);
	}
}

void ABattery::ActivateState(bool bInActive, AActor* Target)
{
	if (bActive == bInActive)
		return;

	bActive = bInActive;
	TargetActor = Target;

	StaticMeshComponent->SetMaterial(0,bActive?ActiveMaterial:NormalMaterial);
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalController.h"

#include "BehaviorTree/BehaviorTree.h"
#include "Navigation/PathFollowingComponent.h"


ABaseAnimalController::ABaseAnimalController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ABaseAnimalController::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (auto LoadedBT = Cast<UBehaviorTree>(StaticLoadObject(
		UBehaviorTree::StaticClass(),
		nullptr,
		TEXT("/Game/AI/BehaviorTrees/BT_AnimalBase.BT_AnimalBase"))))
	{
		RunBehaviorTree(LoadedBT);
	}
}

void ABaseAnimalController::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseAnimalController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
	auto Path = GetPathFollowingComponent()->GetPath();
	if (!Path.IsValid()) return;
	
	const TArray<FNavPathPoint>& Points = Path->GetPathPoints();

	for (int i = 0; i < Points.Num() - 1; i++)
	{
		DrawDebugLine(
			GetWorld(),
			Points[i].Location,
			Points[i + 1].Location,
			FColor::Blue,
			false,
			-1,
			0,
			3
		);
	}
}

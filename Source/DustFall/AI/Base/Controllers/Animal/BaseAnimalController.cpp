// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalController.h"

#include "BehaviorTree/BehaviorTree.h"


ABaseAnimalController::ABaseAnimalController()
{
	PrimaryActorTick.bCanEverTick = false;
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

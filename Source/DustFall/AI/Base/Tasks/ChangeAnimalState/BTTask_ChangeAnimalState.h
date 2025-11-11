// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlueprintBase.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "BTTask_ChangeAnimalState.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTTask_ChangeAnimalState : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ChangeAnimalState();

	UPROPERTY(EditAnywhere, Category = "Animal")
	EAnimalState NewState;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

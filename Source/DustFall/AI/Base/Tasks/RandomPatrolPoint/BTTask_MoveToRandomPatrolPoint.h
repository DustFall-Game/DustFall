// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BTTaskNode.h"
#include "Components/BoxComponent.h"
#include "Navigation/PathFollowingComponent.h"
#include "BTTask_MoveToRandomPatrolPoint.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTTask_MoveToRandomPatrolPoint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_MoveToRandomPatrolPoint();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_SetBlackboardBool.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTTask_SetBlackboardBool : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_SetBlackboardBool();
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector BlackboardKey;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	bool NewValue = true;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

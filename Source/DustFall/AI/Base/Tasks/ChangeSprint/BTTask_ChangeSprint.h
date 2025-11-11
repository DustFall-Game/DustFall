// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ChangeSprint.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTTask_ChangeSprint : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ChangeSprint();

	UPROPERTY(EditAnywhere, Category = "Movement")
	bool bIsNewSprint;
	
protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};

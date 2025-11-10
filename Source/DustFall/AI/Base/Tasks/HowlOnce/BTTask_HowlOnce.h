// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_HowlOnce.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTTask_HowlOnce : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_HowlOnce();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	UPROPERTY(EditAnywhere, Category="Animation")
	UAnimMontage* HowlMontage;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BT_ServiceCheckPlayerDistance.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBT_ServiceCheckPlayerDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBT_ServiceCheckPlayerDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

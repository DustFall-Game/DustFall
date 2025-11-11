// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckActorDistance.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UBTService_CheckActorDistance : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckActorDistance();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector TargetActorKey;

	UPROPERTY(EditAnywhere, Category="AI")
	FBlackboardKeySelector DistanceToTargetKey;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToRandomPatrolPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"


UBTTask_MoveToRandomPatrolPoint::UBTTask_MoveToRandomPatrolPoint()
{
	NodeName = "Move To Random Patrol Point";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToRandomPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto AIPawn = AIController ? AIController->GetPawn() : nullptr;
	if (!AIPawn || !AIController) return EBTNodeResult::Failed;

	auto NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	if (!NavigationSystem) return EBTNodeResult::Failed;
	
	FNavLocation RandomLocation;
	if (NavigationSystem->GetRandomReachablePointInRadius(AIPawn->GetActorLocation(), 800.f, RandomLocation))
	{
		CachedOwnerComp = &OwnerComp;
		
		FAIRequestID MoveRequestID = AIController->MoveToLocation(
			RandomLocation.Location, 
			5.0f, 
			true);
		
		if (MoveRequestID.IsValid())
		{
			AIController->ReceiveMoveCompleted.AddDynamic(this, &UBTTask_MoveToRandomPatrolPoint::OnMoveCompleted);
			return EBTNodeResult::InProgress;
		}
	}

	return EBTNodeResult::Failed;
}

void UBTTask_MoveToRandomPatrolPoint::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	if (!CachedOwnerComp) return;

	if (Result == EPathFollowingResult::Success)
	{
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
	} else {
		FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Failed);
	}

	CachedOwnerComp = nullptr;
}

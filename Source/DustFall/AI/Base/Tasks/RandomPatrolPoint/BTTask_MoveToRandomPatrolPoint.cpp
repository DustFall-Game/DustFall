// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_MoveToRandomPatrolPoint.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "DustFall/AI/Base/Interfaces/AIHabitatInterface.h"
#include "DustFall/AI/HabitatZone/AIHabitatZone.h"
#include "Kismet/GameplayStatics.h"


UBTTask_MoveToRandomPatrolPoint::UBTTask_MoveToRandomPatrolPoint()
{
	NodeName = "Move To Random Patrol Point";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MoveToRandomPatrolPoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (!AIController) return EBTNodeResult::Failed;

    APawn* Pawn = AIController->GetPawn();
    if (!Pawn) return EBTNodeResult::Failed;

    UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
    if (!BB) return EBTNodeResult::Failed;

    auto NavSystem = UNavigationSystemV1::GetCurrent(Pawn->GetWorld());
    if (!NavSystem) return EBTNodeResult::Failed;

    FVector ResultLocation = FVector::ZeroVector;
    bool bFound = false;

    if (AAIHabitatZone* Zone = IAIHabitatInterface::Execute_GetHabitatZone(Pawn); Zone && Zone->ZoneBox)
    {
        UBoxComponent* ZoneBox = Zone->ZoneBox;
        const FVector Origin = ZoneBox->GetComponentLocation();
        const FVector Extent = ZoneBox->GetScaledBoxExtent();
        const int MaxAttempts = 40;

        for (int i = 0; i < MaxAttempts; ++i)
        {
            FVector RandomPoint(
                FMath::FRandRange(-Extent.X, Extent.X),
                FMath::FRandRange(-Extent.Y, Extent.Y),
                FMath::FRandRange(-Extent.Z, Extent.Z)
            );
            RandomPoint += Origin;

            FNavLocation NavLoc;
            if (NavSystem->ProjectPointToNavigation(RandomPoint, NavLoc))
            {
                if (ZoneBox->Bounds.GetBox().IsInsideOrOn(NavLoc.Location))
                {
                    ResultLocation = NavLoc.Location;
                    bFound = true;
                    break;
                }
            }
        }
    }
    
    if (!bFound)
    {
        FNavLocation NavLoc;
        if (NavSystem->GetRandomReachablePointInRadius(
            Pawn->GetActorLocation(),
            BB->GetValueAsFloat("PatrolRadius"),
            NavLoc)
        ) {
            ResultLocation = NavLoc.Location;
            bFound = true;
        }
    }
    
    if (bFound)
    {
        BB->SetValueAsVector("PatrolPoint", ResultLocation);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

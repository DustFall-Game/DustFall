// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_HowlOnce.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "Engine/OverlapResult.h"
#include "GameFramework/Character.h"

UBTTask_HowlOnce::UBTTask_HowlOnce()
{
	NodeName = "Howl Once";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_HowlOnce::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	auto AIPawn = AIController ? Cast<ACharacter>(AIController->GetPawn()) : nullptr;
	auto Blackboard = AIController ? AIController->GetBlackboardComponent() : nullptr;
	if (!AIPawn || !AIController || !Blackboard) return EBTNodeResult::Failed;

	if (HowlMontage)
		AIPawn->PlayAnimMontage(HowlMontage);

	Blackboard->SetValueAsBool("bHasHowled", true);
	Blackboard->SetValueAsBool("bIsLeader", true);

	TArray<FOverlapResult> Overlaps;
	auto Sphere = FCollisionShape::MakeSphere(10000.f); // 100 м

	GetWorld()->OverlapMultiByObjectType(
		Overlaps,
		AIPawn->GetActorLocation(),
		FQuat::Identity,
		FCollisionObjectQueryParams(ECC_Pawn),
		Sphere
	);

	AActor* ClosestTeammate = nullptr;
	
	for (auto& Hit : Overlaps)
	{
		auto Other = Hit.GetActor(); if (!Other || Other == AIPawn) continue;
		
		auto TeamInterface = Cast<IGenericTeamAgentInterface>(Other);
		auto SelfInterface = Cast<IGenericTeamAgentInterface>(AIPawn);
		
		if (!TeamInterface || !SelfInterface) continue;
		
		FGenericTeamId OtherTeam = TeamInterface->GetGenericTeamId();
		FGenericTeamId SelfTeam = SelfInterface->GetGenericTeamId();
		
		if (OtherTeam == SelfTeam)
			if (float Distance = FVector::Dist(AIPawn->GetActorLocation(), Other->GetActorLocation()); Distance < FLT_MAX)
			{
				ClosestTeammate = Other;
				
				if (auto OtherController = Cast<AAIController>(Other->GetOwner()))
					if (auto OtherBlacboard = OtherController->GetBlackboardComponent())
					{
						OtherBlacboard->SetValueAsObject("TeammateActor", AIPawn);
						OtherBlacboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));
					}
			}
	}
	
	Blackboard->SetValueAsObject("TeammateActor", ClosestTeammate);

	return EBTNodeResult::Succeeded;
}

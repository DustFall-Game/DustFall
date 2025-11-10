// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckActorDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBTService_CheckActorDistance::UBTService_CheckActorDistance()
{
	NodeName = "Check Actor Distance";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTService_CheckActorDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	auto AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	auto Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;

	auto Character = Cast<ACharacter>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!Character) return;

	float DistanceToTarget = AIPawn->GetDistanceTo(Character);
	Blackboard->SetValueAsFloat(DistanceToTargetKey.SelectedKeyName, DistanceToTarget);
}

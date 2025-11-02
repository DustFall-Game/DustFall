// Fill out your copyright notice in the Description page of Project Settings.


#include "BT_ServiceCheckPlayerDistance.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UBT_ServiceCheckPlayerDistance::UBT_ServiceCheckPlayerDistance()
{
	NodeName = "Check Player Distance";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBT_ServiceCheckPlayerDistance::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	auto AIPawn = AIController->GetPawn();
	if (!AIPawn) return;

	auto Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return;

	auto Character = Cast<ACharacter>(Blackboard->GetValueAsObject("TargetActor"));
	if (!Character) return;

	float DistanceToTarget = AIPawn->GetDistanceTo(Character);
	Blackboard->SetValueAsFloat("DistanceToTarget", DistanceToTarget);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_SetBlackboardBool.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType.h"

UBTTask_SetBlackboardBool::UBTTask_SetBlackboardBool()
{
	NodeName = "Set Blackboard Bool";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_SetBlackboardBool::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	auto Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->SetValueAsBool(BlackboardKey.SelectedKeyName, NewValue);

	return EBTNodeResult::Succeeded;
}

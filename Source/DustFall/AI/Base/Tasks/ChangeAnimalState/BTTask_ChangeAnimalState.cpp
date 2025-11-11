// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeAnimalState.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_ChangeAnimalState::UBTTask_ChangeAnimalState()
{
	NodeName = "Change Animal State";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ChangeAnimalState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	auto Blackboard = AIController->GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->SetValueAsEnum(FName("AnimalState"), static_cast<uint8>(NewState));
	
	return EBTNodeResult::Succeeded;
}

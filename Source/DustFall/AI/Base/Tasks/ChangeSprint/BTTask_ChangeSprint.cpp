// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ChangeSprint.h"
#include "AIController.h"
#include "DustFall/AI/Base/Interfaces/AIMovementInterface.h"


UBTTask_ChangeSprint::UBTTask_ChangeSprint()
{
	NodeName = "Change Sprint";
	bNotifyTick = false;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_ChangeSprint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	auto AIController = OwnerComp.GetAIOwner();
	if (!AIController) return EBTNodeResult::Failed;

	auto Pawn = AIController->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	if (Pawn->Implements<UAIMovementInterface>())
	{
		IAIMovementInterface::Execute_HandleSprint(Pawn, bIsNewSprint);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;;
}

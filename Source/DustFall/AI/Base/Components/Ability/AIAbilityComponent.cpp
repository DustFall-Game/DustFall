// Fill out your copyright notice in the Description page of Project Settings.


#include "AIAbilityComponent.h"
#include "BrainComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionComponent.h"


void UAIAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	AIController = Cast<AAIController>(CharacterRef->GetController());
}

void UAIAbilityComponent::Server_Die()
{
	Super::Server_Die();
	
	if (!AIController || !CharacterRef) return;
	
	if (auto Brain = AIController->GetBrainComponent())
		Brain->StopLogic("Died");

	if (auto Perception = AIController->GetPerceptionComponent())
		Perception->Deactivate();

	AIController->UnPossess();

	FTimerHandle Handle;
	GetWorld()->GetTimerManager().SetTimer(
		Handle,
		[this]
		{
			CharacterRef->Destroy();
		},
		45.f,
		false);
}
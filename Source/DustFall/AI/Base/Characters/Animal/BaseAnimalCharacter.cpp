// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DustFall/AI/Base/Components/Ability/AIAbilityComponent.h"
#include "DustFall/AI/Base/Controllers/Animal/BaseAnimalController.h"
#include "DustFall/AI/DataAssets/AnimalDataAsset.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISense_Sight.h"


ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ABaseAnimalController::StaticClass();
	AbilityComponent = CreateDefaultSubobject<UAIAbilityComponent>(TEXT("AbilityComponent"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
}

void ABaseAnimalCharacter::HandleSprint_Implementation(bool bIsSprint)
{
	if (!AnimalDataAsset) return;

	if (auto MovementComponent = GetCharacterMovement())
		if (bIsSprint)
			MovementComponent->MaxWalkSpeed = AnimalDataAsset->SprintSpeed;
		else
			MovementComponent->MaxWalkSpeed = AnimalDataAsset->WalkSpeed;
}

void ABaseAnimalCharacter::GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const
{
	Super::GetActorEyesViewPoint(OutLocation, OutRotation);

	auto SM = GetMesh();
	if (SM && SM->DoesSocketExist("HeadSocket"))
	{
		OutLocation = SM->GetSocketLocation("HeadSocket");
		OutRotation = SM->GetSocketRotation("HeadSocket");
	}
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAnimalCharacter::OnPerceptionUpdated);
}

void ABaseAnimalCharacter::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	auto Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	if (!Blackboard) return;
	
	FString SenseName = Stimulus.Type.Name.ToString();
	if (SenseName.Contains("AISense_Sight"))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			Blackboard->SetValueAsObject("TargetActor", Actor);
			Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));
		} else {
			Blackboard->ClearValue("TargetActor");
			
			if (Blackboard->GetValueAsEnum("AnimalState") != static_cast<uint8>(EAnimalState::Flee))
				Blackboard->ClearValue("AnimalState");
		}
	}
	else if (SenseName.Contains("AISense_Hearing"))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogTemp, Display, TEXT("Hearing detected: %s"), *Actor->GetName());
		} else {
			UE_LOG(LogTemp, Display, TEXT("Hearing lost: %s"), *Actor->GetName());
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DustFall/AI/Base/Components/Ability/AIAbilityComponent.h"
#include "DustFall/AI/Base/Controllers/Animal/BaseAnimalController.h"
#include "DustFall/AI/DataAssets/AnimalDataAsset.h"
#include "DustFall/AI/Enums/AnimalType.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "DustFall/Core/Player/Character/DF_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AIControllerClass = ABaseAnimalController::StaticClass();
	AbilityComponent = CreateDefaultSubobject<UAIAbilityComponent>(TEXT("AbilityComponent"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (GetController())
		Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
	
	if (Blackboard)
		Blackboard->SetValueAsEnum("AnimalType",
			static_cast<uint8>(TeamToAnimal(AnimalDataAsset ? AnimalDataAsset->TeamType : ETeamType::None))
		);

	if (!AnimalDataAsset || !PerceptionComponent) return;
	
	SightConfig->SightRadius = AnimalDataAsset->SightRadius;
	SightConfig->LoseSightRadius = AnimalDataAsset->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AnimalDataAsset->PeripheralVision;

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAnimalCharacter::OnPerceptionUpdated);
	AbilityComponent->OnDamageTaken.AddDynamic(this, &ABaseAnimalCharacter::OnDamageTaken);
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

void ABaseAnimalCharacter::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
    if (!Actor || !Blackboard || !AnimalDataAsset || !Cast<ADF_PlayerCharacter>(Actor)) return;

    FString SenseName = Stimulus.Type.Name.ToString();
    if (SenseName.Contains("AISense_Sight") && AnimalDataAsset->bReactToSight)
    {
    	if (Stimulus.WasSuccessfullySensed())
    	{
    		if (!Blackboard->GetValueAsObject("TargetActor"))
    			Blackboard->SetValueAsObject("TargetActor", Actor);
    		
    		if (Blackboard->GetValueAsEnum("AnimalState") != static_cast<uint8>(EAnimalState::Flee))
    			Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));
    	}
    	
    	else if (Blackboard->GetValueAsObject("TargetActor") == Actor)
        {
  			Blackboard->ClearValue("TargetActor");
	  
        	if (Blackboard->GetValueAsEnum("AnimalState") != static_cast<uint8>(EAnimalState::Flee))
        		Blackboard->ClearValue("AnimalState");      		
        }
    }
    
    else if (SenseName.Contains("AISense_Hearing") && AnimalDataAsset->bReactToHearing)
    {
        if (Stimulus.WasSuccessfullySensed())
            Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));
    }
}

void ABaseAnimalCharacter::OnDamageTaken()
{
	if (!Blackboard) return;
	
	if (AnimalDataAsset->bReactToDamage)
		Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Flee));
	else
		Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));

	if (AnimalDataAsset->bAttackOnDamage)
		Blackboard->SetValueAsBool("bCanAttack", true);
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

EAnimalType ABaseAnimalCharacter::TeamToAnimal(ETeamType Team)
{
	switch (Team)
	{
		case ETeamType::Fox: return EAnimalType::Fox;
		case ETeamType::Pig: return EAnimalType::Pig;
		case ETeamType::Deer: return EAnimalType::Deer;
		case ETeamType::Wolf: return EAnimalType::Wolf;
		case ETeamType::Bear: return EAnimalType::Bear;
		case ETeamType::Boar: return EAnimalType::Boar;
		case ETeamType::Rabbit: return EAnimalType::Rabbit;

		default: return EAnimalType::None;
	}
}

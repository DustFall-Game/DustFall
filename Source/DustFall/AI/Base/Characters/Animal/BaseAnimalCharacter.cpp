// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "DustFall/AI/Base/AnimInstance/BaseAnimalAnimInstance.h"
#include "DustFall/AI/Base/Components/Ability/AIAbilityComponent.h"
#include "DustFall/AI/Base/Controllers/Animal/BaseAnimalController.h"
#include "DustFall/AI/DataAssets/AnimalDataAsset.h"
#include "DustFall/AI/Enums/AnimalType.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "DustFall/Core/Player/Character/DF_PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


ABaseAnimalCharacter::ABaseAnimalCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	bUseControllerRotationYaw = false;

	AIControllerClass = ABaseAnimalController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	if (auto MovementComponent = GetCharacterMovement()) {
		MovementComponent->bOrientRotationToMovement = true;
		MovementComponent->RotationRate = FRotator(0.f, 180.f, 0.f);
	}
	
	AbilityComponent = CreateDefaultSubobject<UAIAbilityComponent>(TEXT("AbilityComponent"));
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));

	DetectSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DetectSphere"));
	DetectSphere->SetupAttachment(RootComponent);
	DetectSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	DetectSphere->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	DetectSphere->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	DetectSphere->InitSphereRadius(150.f);
}

void ABaseAnimalCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (auto AIC = GetController())
	{
		Blackboard = Cast<AAIController>(AIC)->GetBlackboardComponent();
		
		if (auto AnimInstance = Cast<UBaseAnimalAnimInstance>(GetMesh()->GetAnimInstance()))
			AnimInstance->InitializeBlackboard(Blackboard);
	}
	
	if (Blackboard) {
		Blackboard->SetValueAsFloat("PatrolRadius", AnimalDataAsset->PatrolRadius);
		Blackboard->SetValueAsEnum("AnimalType",
			static_cast<uint8>(TeamToAnimal(AnimalDataAsset ? AnimalDataAsset->TeamType : ETeamType::None)));
	}

	if (!AnimalDataAsset || !PerceptionComponent || !DetectSphere) return;
	
	SightConfig->SightRadius = AnimalDataAsset->SightRadius;
	SightConfig->LoseSightRadius = AnimalDataAsset->LoseSightRadius;
	SightConfig->PeripheralVisionAngleDegrees = AnimalDataAsset->PeripheralVision;
	SightConfig->SetMaxAge(3.f);

	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = false;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = false;
	
	PerceptionComponent->ConfigureSense(*SightConfig);
	PerceptionComponent->SetDominantSense(SightConfig->GetSenseImplementation());

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ABaseAnimalCharacter::OnPerceptionUpdated);
	AbilityComponent->OnDamageTaken.AddDynamic(this, &ABaseAnimalCharacter::OnDamageTaken);
	DetectSphere->OnComponentBeginOverlap.AddDynamic(this, &ABaseAnimalCharacter::OnDetectOverlap);

	DetectSphere->SetSphereRadius(AnimalDataAsset->DetectRadius);

	if (auto MovementComponent = GetCharacterMovement())
		MovementComponent->MaxWalkSpeed = AnimalDataAsset->WalkSpeed;

	if (AnimalDataAsset->TeamType == ETeamType::Bear || AnimalDataAsset->TeamType == ETeamType::Wolf)
	{
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			this,
			&ABaseAnimalCharacter::OnDetectedTargetActor,
			0.5f,
			true
		);
	}
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

void ABaseAnimalCharacter::DoAttackTrace_Implementation(FName DamageSourceBone)
{
	const USkeletalMeshComponent* SKMesh = GetMesh();
	if (!SKMesh || !AnimalDataAsset || !Blackboard || !GetWorld()) return;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_Pawn));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECC_WorldDynamic));
	
	const FTransform SocketTransform = SKMesh->GetSocketTransform(DamageSourceBone);

	const FVector StartLocation = SocketTransform.GetLocation();
	const FVector EndLocation =
		StartLocation + SocketTransform.GetRotation().GetForwardVector()
		* AnimalDataAsset->MeleeTraceDistance;
	
	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMultiForObjects(
		GetWorld(),
		StartLocation,
		EndLocation,
		AnimalDataAsset->MeleeTraceRadius,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
	
	if (bHit && HasAuthority())
		for (const FHitResult& Hit : HitResults)
		{
			AActor* HitActor = Hit.GetActor();
			if (!HitActor || !Cast<ADF_PlayerCharacter>(HitActor)) continue;

			auto ABComponent = HitActor->FindComponentByClass<UBaseAbilityComponent>();
			if (!ABComponent) continue;

			if (ABComponent->GetHealth() <= 0) continue;
			
			IDamageInterface::Execute_TakeDamage(
				ABComponent,
				AnimalDataAsset->MeleeDamage,
				this,
				Hit.BoneName
			);

			if (ABComponent->GetHealth() <= 0)
				Blackboard->ClearValue(FName(TEXT("TargetActor")));
		}
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

void ABaseAnimalCharacter::OnDetectedTargetActor()
{
	if (bIsAttacked && AbilityComponent->GetHealth() > 0) return;

	const FVector StartLocation = GetMesh()->GetSocketLocation("SKT_Neck");
	const FVector EndLocation = StartLocation + GetActorForwardVector() * AnimalDataAsset->MeleeTraceDistanceDetected;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<FHitResult> HitResults;
	bool bHit = UKismetSystemLibrary::SphereTraceMulti(
		GetWorld(),
		StartLocation,
		EndLocation,
		15.f,
		UEngineTypes::ConvertToTraceType(ECC_Visibility),
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true);

	if (!bHit) return;

	for (const FHitResult& Hit : HitResults)
	{
		if (Cast<ADF_PlayerCharacter>(Hit.GetActor()))
			StartAttack();
	}
}

void ABaseAnimalCharacter::StartAttack()
{
	if (bIsAttacked) return;

	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	if (!Anim || !AttackMontage) return;

	bIsAttacked = true;

	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ABaseAnimalCharacter::OnAttackMontageEnded);

	Anim->Montage_Play(AttackMontage, 1.f);
	Anim->Montage_SetEndDelegate(EndDelegate, AttackMontage);
}


void ABaseAnimalCharacter::OnAttackMontageEnded(UAnimMontage* Montage, bool bInterrupted)
{
	bIsAttacked = false;
}

void ABaseAnimalCharacter::OnDetectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                           UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor || OtherActor == this || !Cast<ADF_PlayerCharacter>(OtherActor) || !Blackboard) return;

	if (!Blackboard->GetValueAsObject("TargetActor"))
		Blackboard->SetValueAsObject("TargetActor", OtherActor);
    		
	if (Blackboard->GetValueAsEnum("AnimalState") != static_cast<uint8>(EAnimalState::Flee))
		Blackboard->SetValueAsEnum("AnimalState", static_cast<uint8>(EAnimalState::Alert));
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

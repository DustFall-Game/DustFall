// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "DustFall/AI/Base/Interfaces/AIHabitatInterface.h"
#include "DustFall/AI/Base/Interfaces/AIMovementInterface.h"
#include "DustFall/AI/DataAssets/AnimalDataAsset.h"
#include "DustFall/AI/Enums/AnimalType.h"
#include "DustFall/AI/Enums/TeamType.h"
#include "GameFramework/Character.h"
#include "BaseAnimalCharacter.generated.h"

class UAISenseConfig_Hearing;
class UAISenseConfig_Sight;
class UAnimalDataAsset;
struct FAIStimulus;
class UAIPerceptionComponent;
class UAIAbilityComponent;

UCLASS()
class DUSTFALL_API ABaseAnimalCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAIMovementInterface, public IAIHabitatInterface
{
	GENERATED_BODY()

public:
	ABaseAnimalCharacter();
	
	/** Interfaces */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;
	virtual void SetHabitatZone_Implementation(AAIHabitatZone* Zone) override { HabitatZone = Zone; };
	virtual AAIHabitatZone* GetHabitatZone_Implementation() override { return HabitatZone; };

protected:
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	UFUNCTION()
	virtual void OnDamageTaken();

	UFUNCTION()
	void OnDetectOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
		bool bFromSweep, const FHitResult& SweepResult);

	/** References */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBlackboardComponent* Blackboard;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIAbilityComponent* AbilityComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USphereComponent* DetectSphere;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;

	UPROPERTY(EditAnywhere, Category="Assets")
	UAnimalDataAsset* AnimalDataAsset;
	
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(static_cast<uint8>(AnimalDataAsset ? AnimalDataAsset->TeamType : ETeamType::None));
	}

private:
	static EAnimalType TeamToAnimal(ETeamType Team);
	
	UPROPERTY()
	AAIHabitatZone* HabitatZone;
};

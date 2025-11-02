// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GenericTeamAgentInterface.h"
#include "DustFall/AI/Base/Interfaces/AIMovementInterface.h"
#include "GameFramework/Character.h"
#include "BaseAnimalCharacter.generated.h"

class UAnimalDataAsset;
struct FAIStimulus;
class UAIPerceptionComponent;
class UAIAbilityComponent;

UCLASS()
class DUSTFALL_API ABaseAnimalCharacter : public ACharacter, public IGenericTeamAgentInterface, public IAIMovementInterface
{
	GENERATED_BODY()

public:
	ABaseAnimalCharacter();

	/** Interfaces */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetActorEyesViewPoint(FVector& OutLocation, FRotator& OutRotation) const override;

	UFUNCTION()
	virtual void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

	/** References */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIAbilityComponent* AbilityComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAIPerceptionComponent* PerceptionComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimalDataAsset* AnimalDataAsset;

	UPROPERTY(EditAnywhere, Category="Team")
	uint8 TeamIdValue = 2;
	
	virtual FGenericTeamId GetGenericTeamId() const override
	{
		return FGenericTeamId(TeamIdValue);
	}
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustFall/Characters/Player/Interfaces/InputToPlayer/InputToPlayerInterface.h"
#include "DF_AbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UDF_AbilityComponent : public UActorComponent, public IInputToPlayerInterface
{
	GENERATED_BODY()

public:
	UDF_AbilityComponent();

	/** Implements */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;

protected:
	virtual void BeginPlay() override;

};

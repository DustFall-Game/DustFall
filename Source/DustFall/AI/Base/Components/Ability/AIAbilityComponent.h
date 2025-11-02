// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Components/ActorComponent.h"
#include "DustFall/Core/Components/Ability/BaseAbilityComponent.h"
#include "AIAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UAIAbilityComponent : public UBaseAbilityComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void Server_Die() override;

	/** References */
	UPROPERTY()
	AAIController* AIController;
};

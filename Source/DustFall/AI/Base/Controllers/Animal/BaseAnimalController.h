// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "BaseAnimalController.generated.h"

UCLASS()
class DUSTFALL_API ABaseAnimalController : public AAIController
{
	GENERATED_BODY()

public:
	ABaseAnimalController();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
};

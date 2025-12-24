// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AttackerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAttackerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSTFALL_API IAttackerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	void DoAttackTrace(FName DamageSourceBone);
};

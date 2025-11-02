// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIMovementInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAIMovementInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSTFALL_API IAIMovementInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	void HandleSprint(bool bIsSprint);
};

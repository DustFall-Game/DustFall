// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "AIHabitatInterface.generated.h"

class AAIHabitatZone;
// This class does not need to be modified.
UINTERFACE()
class UAIHabitatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSTFALL_API IAIHabitatInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	AAIHabitatZone* GetHabitatZone();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	void SetHabitatZone(AAIHabitatZone* Zone);
};

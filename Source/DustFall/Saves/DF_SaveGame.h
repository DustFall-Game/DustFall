// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "DF_SaveGame.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UDF_SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadWrite)
	FString SaveName;

	UPROPERTY(BlueprintReadWrite)
	FDateTime SaveDate;

	UPROPERTY(BlueprintReadWrite)
	FString ScreenshotPath;

	UPROPERTY(BlueprintReadWrite)
	int32 PlayerLevel;

	UPROPERTY(BlueprintReadOnly)
	FVector PlayerLocation;
};

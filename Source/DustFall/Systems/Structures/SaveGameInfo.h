#pragma once

#include "CoreMinimal.h"
#include "SaveGameInfo.generated.h"

USTRUCT(BlueprintType)
struct FSaveGameInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString SaveName;

	UPROPERTY(BlueprintReadOnly)
	FDateTime SaveDate;

	UPROPERTY(BlueprintReadOnly)
	UTexture2D* SaveScreenshot;
};

USTRUCT(BlueprintType)
struct FNewSaveGameInfo
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadOnly)
	FString SaveSlotName;

	UPROPERTY(BlueprintReadOnly)
	int32 PlayerLevel;

	UPROPERTY(BlueprintReadOnly)
	FVector PlayerLocation;
};

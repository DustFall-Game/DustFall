﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/Systems/Structures/SaveGameInfo.h"
#include "UObject/Object.h"
#include "SaveGameManager.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API USaveGameManager : public UObject
{
	GENERATED_BODY()

public:
	static void SaveGame(FNewSaveGameInfo NewSaveGameInfo);
	static TArray<FSaveGameInfo> GetAllSaveGames();
	static int32 GetLevelIndex(UWorld* World);

private:
	static void SaveScreenshotToFile(const FString& FilePath);
	static UTexture2D* LoadTextureFromFile(const FString& ImagePath);
};

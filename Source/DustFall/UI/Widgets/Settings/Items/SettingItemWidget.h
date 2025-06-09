﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SettingItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API USettingItemWidget : public UUserWidget
{
	GENERATED_BODY()

	protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Settings", meta = (ExposeOnSpawn))
	FString Title;
};

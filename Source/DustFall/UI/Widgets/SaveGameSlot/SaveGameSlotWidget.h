// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveGameSlotWidget.generated.h"

class UImage;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DUSTFALL_API USaveGameSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Binds */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SaveName;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SaveData;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_SaveTimestamp;
	
	UPROPERTY(meta = (BindWidget))
	UImage* Img_SaveScreenshot;
};

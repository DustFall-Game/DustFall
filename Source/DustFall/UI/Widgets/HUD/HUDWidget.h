// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/UI/Widgets/BaseUserWidget/BaseUserWidget.h"
#include "HUDWidget.generated.h"

class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UHUDWidget : public UBaseUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void HandleStatChanged(FName StatName, float NewValue);

	/** Bindings */
	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category="HUD")
	UTextBlock* Text_Bleeding;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite, Category="HUD")
	UProgressBar* PB_Stamina;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_Health;

	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_Satiety;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* PB_Thirst;
};

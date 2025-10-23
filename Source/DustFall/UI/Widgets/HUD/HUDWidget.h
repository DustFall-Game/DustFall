// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/UI/Widgets/BaseUserWidget/BaseUserWidget.h"
#include "HUDWidget.generated.h"

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
};

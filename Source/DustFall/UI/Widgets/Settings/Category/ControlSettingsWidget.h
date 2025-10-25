// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlSettingsWidget.generated.h"

class UDF_UserSettings;
class USettingItem_Slider;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UControlSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Binds */
	UPROPERTY(meta = (BindWidget))
	USettingItem_Slider* Slider_MouseSensitivity;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChangeMouseSensitivity(float Value);

	UFUNCTION()
	void SaveSettings();

	UPROPERTY()
	UDF_UserSettings* UserSettings;

private:
	FTimerHandle SaveTimerHandle;
};

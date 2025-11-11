// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlSettingsWidget.generated.h"

class UVerticalBox;
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

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VB_ControlSettings;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChangeMouseSensitivity(float Value);

	UFUNCTION()
	void SaveSettings();

	/** References */
	UPROPERTY()
	UDF_UserSettings* UserSettings;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> KeySelectorWidgetClass;

private:
	FTimerHandle SaveTimerHandle;
};

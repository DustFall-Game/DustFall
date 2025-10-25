// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlSettingsWidget.h"
#include "DustFall/Systems/UserSettings/DF_UserSettings.h"
#include "DustFall/UI/Widgets/Settings/Items/SettingItem_Slider.h"

void UControlSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UserSettings = Cast<UDF_UserSettings>(GEngine->GetGameUserSettings());
	if (!UserSettings) return;

	UserSettings->LoadSettings();
	if (Slider_MouseSensitivity)
	{
		Slider_MouseSensitivity->SetMinValue(0.2f);
		Slider_MouseSensitivity->SetValue(UserSettings->GetMouseSensitivity());
		
		Slider_MouseSensitivity->OnValueChanged.AddDynamic(this, &UControlSettingsWidget::OnChangeMouseSensitivity);
	}
}

void UControlSettingsWidget::OnChangeMouseSensitivity(float Value)
{
	UserSettings->SetMouseSensitivity(Slider_MouseSensitivity->GetValue());
	
	GetWorld()->GetTimerManager().ClearTimer(SaveTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(SaveTimerHandle, this, &UControlSettingsWidget::SaveSettings, 0.5f, false);
}

void UControlSettingsWidget::SaveSettings()
{
	if (!UserSettings) return;
	
	UserSettings->ApplySettings(false);
	UserSettings->SaveSettings();
}
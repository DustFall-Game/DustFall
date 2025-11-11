// Fill out your copyright notice in the Description page of Project Settings.


#include "ControlSettingsWidget.h"
#include "EnhancedInputSubsystems.h"
#include "Components/VerticalBox.h"
#include "DustFall/Systems/UserSettings/DF_UserSettings.h"
#include "DustFall/UI/Widgets/Settings/Items/SettingItem_KeySelector.h"
#include "DustFall/UI/Widgets/Settings/Items/SettingItem_Slider.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void UControlSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	UserSettings = Cast<UDF_UserSettings>(GEngine->GetGameUserSettings());
	if (!UserSettings) return;

	/** User Input Settings */
	if (ULocalPlayer* LocalPlayer = GetOwningPlayer()->GetLocalPlayer())
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			auto InputUserSettings = Subsystem->GetUserSettings();
			auto ActiveProfile = InputUserSettings->GetActiveKeyProfile();
			if (!ActiveProfile) return;
			
			auto& KeyMappingRowsMap = ActiveProfile->GetPlayerMappingRows();

			TArray<FKeyMappingRow> KeyMappingArray;
			KeyMappingRowsMap.GenerateValueArray(KeyMappingArray);
			
			for (auto Row : KeyMappingArray)
			{
				TArray<FPlayerKeyMapping> MappingsArray = Row.Mappings.Array();
				FPlayerKeyMapping MappingFound;

				
				for (auto Mapping : MappingsArray)
				{
					if (Mapping.GetSlot() == EPlayerMappableKeySlot::First)
					{
						MappingFound = Mapping;
						
						break;
					}
				}

				if (!MappingFound.IsValid() || !KeySelectorWidgetClass || !VB_ControlSettings) break;

				auto Widget = CreateWidget<USettingItem_KeySelector>(this, KeySelectorWidgetClass);
				
				Widget->KeyMap = MappingFound;
				Widget->UserSettings = InputUserSettings;

				VB_ControlSettings->AddChild(Widget);
			}
		}

	/** User Game Settings */
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
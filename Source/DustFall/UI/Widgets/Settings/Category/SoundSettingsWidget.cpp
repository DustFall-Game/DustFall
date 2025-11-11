// Fill out your copyright notice in the Description page of Project Settings.


#include "SoundSettingsWidget.h"
#include "DustFall/Systems/UserSettings/DF_UserSettings.h"
#include "DustFall/UI/Widgets/Settings/Items/SettingItem_Slider.h"
#include "Kismet/GameplayStatics.h"

void USoundSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	UserSettings = Cast<UDF_UserSettings>(GEngine->GetGameUserSettings());
	if (!UserSettings) return;

	UserSettings->LoadSettings();
	if (Slider_MasterVolume)
	{
		Slider_MasterVolume->SetValue(UserSettings->GetMasterVolume());
		Slider_MasterVolume->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnChangeSettingsVolume);
	}
	
	if (Slider_VoiceVolume)
	{
		Slider_VoiceVolume->SetValue(UserSettings->GetVoiceVolume());
		Slider_VoiceVolume->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnChangeSettingsVolume);
	}
	
	if (Slider_SFXVolume)
	{
		Slider_SFXVolume->SetValue(UserSettings->GetSFXVolume());
		Slider_SFXVolume->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnChangeSettingsVolume);
	}
	
	if (Slider_AmbientVolume)
	{
		Slider_AmbientVolume->SetValue(UserSettings->GetAmbientVolume());
		Slider_AmbientVolume->OnValueChanged.AddDynamic(this, &USoundSettingsWidget::OnChangeSettingsVolume);
	}
}

void USoundSettingsWidget::OnChangeSettingsVolume(float Value)
{
	if (!MainSoundMix) return;

	const auto UpdateVolume = [&](USoundClass* SoundClass, float NewValue, float (UDF_UserSettings::*Getter)() const, void (UDF_UserSettings::*Setter)(float))
	{
		if (!SoundClass) return;

		const float CurrentValue = (UserSettings->*Getter)();
		if (FMath::IsNearlyEqual(CurrentValue, NewValue, 0.001f))
			return;

		UGameplayStatics::SetSoundMixClassOverride(GetWorld(), MainSoundMix, SoundClass, NewValue, 1.f, true);
		(UserSettings->*Setter)(NewValue);
	};
	
	UpdateVolume(MasterVolumeClass, Slider_MasterVolume ? Slider_MasterVolume->GetValue() : UserSettings->GetMasterVolume(), &UDF_UserSettings::GetMasterVolume, &UDF_UserSettings::SetMasterVolume);
	UpdateVolume(VoiceVolumeClass, Slider_VoiceVolume ? Slider_VoiceVolume->GetValue() : UserSettings->GetVoiceVolume(), &UDF_UserSettings::GetVoiceVolume, &UDF_UserSettings::SetVoiceVolume);
	UpdateVolume(SFXVolumeClass, Slider_SFXVolume ? Slider_SFXVolume->GetValue() : UserSettings->GetSFXVolume(), &UDF_UserSettings::GetSFXVolume, &UDF_UserSettings::SetSFXVolume);
	UpdateVolume(AmbientVolumeClass, Slider_AmbientVolume ? Slider_AmbientVolume->GetValue() : UserSettings->GetAmbientVolume(), &UDF_UserSettings::GetAmbientVolume, &UDF_UserSettings::SetAmbientVolume);

	UGameplayStatics::PushSoundMixModifier(GetWorld(), MainSoundMix);
	
	GetWorld()->GetTimerManager().ClearTimer(SaveTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(SaveTimerHandle, this, &USoundSettingsWidget::SaveSettings, 0.5f, false);
}

void USoundSettingsWidget::SaveSettings()
{
	if (!UserSettings) return;

	UE_LOG(LogTemp, Warning, TEXT("USoundSettingsWidget::SaveSettings"));
	UserSettings->ApplySettings(false);
	UserSettings->SaveSettings();
}
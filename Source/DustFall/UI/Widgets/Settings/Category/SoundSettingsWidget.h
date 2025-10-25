// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SoundSettingsWidget.generated.h"

class UDF_UserSettings;
class USettingItem_Slider;
/**
 * 
 */
UCLASS()
class DUSTFALL_API USoundSettingsWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	/** Binds */
	UPROPERTY(meta = (BindWidget))
	USettingItem_Slider* Slider_MasterVolume;

	UPROPERTY(meta = (BindWidget))
	USettingItem_Slider* Slider_VoiceVolume;

	UPROPERTY(meta = (BindWidget))
	USettingItem_Slider* Slider_SFXVolume;

	UPROPERTY(meta = (BindWidget))
	USettingItem_Slider* Slider_AmbientVolume;

	/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* MasterVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* VoiceVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SFXVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* AmbientVolumeClass;

protected:
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnChangeSettingsVolume(float Value);

	UFUNCTION()
	void SaveSettings();

	UPROPERTY()
	UDF_UserSettings* UserSettings;

private:
	FTimerHandle SaveTimerHandle;
};

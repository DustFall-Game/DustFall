// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "DF_UserSettings.generated.h"

/**
 * 
 */
UCLASS(Config=GameUserSettings, configdonotcheckdefaults, Blueprintable)
class DUSTFALL_API UDF_UserSettings : public UGameUserSettings
{
	GENERATED_BODY()
	
public:
	UDF_UserSettings(const FObjectInitializer& ObjectInitializer);
	
	/** Getters */
	float GetMouseSensitivity() const { return MouseSensitivity; }
	float GetMasterVolume() const { return MasterVolume; }
	float GetVoiceVolume() const { return VoiceVolume; }
	float GetSFXVolume() const { return SFXVolume; }
	float GetAmbientVolume() const { return AmbientVolume; }

	/** Setters */
	void SetMouseSensitivity(float Value);
	void SetMasterVolume(float Value);
	void SetVoiceVolume(float Value);
	void SetSFXVolume(float Value);
	void SetAmbientVolume(float Value);

private:
	UPROPERTY(Config)
	float MouseSensitivity;

	UPROPERTY(Config)
	float MasterVolume;

	UPROPERTY(Config)
	float VoiceVolume;

	UPROPERTY(Config)
	float SFXVolume;

	UPROPERTY(Config)
	float AmbientVolume;
};

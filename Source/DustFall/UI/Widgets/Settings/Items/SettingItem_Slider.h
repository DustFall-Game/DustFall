// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItemWidget.h"
#include "SettingItem_Slider.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnValueChanged, float, value);

/**
 * 
 */
UCLASS()
class DUSTFALL_API USettingItem_Slider : public USettingItemWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class USlider* Slider;

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnValueChanged OnValueChanged;
	
	UFUNCTION()
	void SetValue(float value);

	UFUNCTION()
	void SetMinValue(float value);

	UFUNCTION()
	void SetMaxValue(float value);

	UFUNCTION()
	float GetValue();

private:
	UFUNCTION()
	void HandleValueChanged(float value);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItemWidget.h"
#include "SettingItem_CheckBox.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCheckBoxChanged, bool, value);

/**
 * 
 */
UCLASS()
class DUSTFALL_API USettingItem_CheckBox : public USettingItemWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UCheckBox* CheckBox;

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;


public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnCheckBoxChanged OnCheckBoxChanged;
	
	UFUNCTION()
	void SetValue(bool value);

	UFUNCTION()
	bool GetValue();

private:
	UFUNCTION()
	void HandleCheckBoxChanged(bool value);
};

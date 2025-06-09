// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItemWidget.h"
#include "SettingItem_ComboBox.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnComboBoxSelectionChanged, const FString&, SelectedItem);

UCLASS()
class DUSTFALL_API USettingItem_ComboBox : public USettingItemWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UComboBoxString* ComboBox;

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;
	
public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnComboBoxSelectionChanged OnSelectionChanged;
	
	UFUNCTION()
	void SetItems(TArray<FString> ItemsSource);

	UFUNCTION()
	void SetDefaultValue(FString SelectedItem);

private:
	UFUNCTION()
	void HandleSelectionChanged(FString SelectedItem, ESelectInfo::Type SelectInfo);
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UserSettings/EnhancedInputUserSettings.h"
#include "SettingItem_KeySelector.generated.h"


class UEnhancedInputUserSettings;
class UInputKeySelector;
class UButton;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DUSTFALL_API USettingItem_KeySelector : public UUserWidget
{
	GENERATED_BODY()

public:
	/** References */
	UPROPERTY()
	FPlayerKeyMapping KeyMap;

	UPROPERTY()
	UEnhancedInputUserSettings* UserSettings;
	
protected:
	virtual void NativeConstruct() override;

	/** Binds */
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Title;

	UPROPERTY(meta = (BindWidget))
	UInputKeySelector* IKey_Selector;

	UPROPERTY(meta = (BindWidget))
	UButton* Btn_Reset;

private:
	/** State */
	bool bIsSelectedChanged;

	/** Handlers */
	UFUNCTION()
	void HandleChangeInputKeySelector(FInputChord SelectedKey);

	UFUNCTION()
	void HandleSelectedInputKeySelector();
	
	UFUNCTION()
	void HandleUpdate(FKey Key);

	UFUNCTION()
	void HandleReset();
};

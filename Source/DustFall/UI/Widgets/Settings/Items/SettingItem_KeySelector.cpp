// Fill out your copyright notice in the Description page of Project Settings.


#include "SettingItem_KeySelector.h"
#include "Components/Button.h"
#include "Components/InputKeySelector.h"
#include "Components/TextBlock.h"
#include "UserSettings/EnhancedInputUserSettings.h"

void USettingItem_KeySelector::NativeConstruct()
{
	Super::NativeConstruct();

	HandleUpdate(KeyMap.GetCurrentKey());

	if (Text_Title)
		Text_Title->SetText(KeyMap.GetDisplayName());

	if (IKey_Selector)
	{
		IKey_Selector->OnKeySelected.AddDynamic(this, &USettingItem_KeySelector::HandleChangeInputKeySelector);
		IKey_Selector->OnIsSelectingKeyChanged.AddDynamic(this, &USettingItem_KeySelector::HandleSelectedInputKeySelector);
	}

	if (Btn_Reset)
		Btn_Reset->OnClicked.AddDynamic(this, &USettingItem_KeySelector::HandleReset);
}

void USettingItem_KeySelector::HandleChangeInputKeySelector(FInputChord SelectedKey)
{
	if (!bIsSelectedChanged) return;
	bIsSelectedChanged = false;
	
	FMapPlayerKeyArgs MapPlayerKey;
	FGameplayTagContainer FailureReason;
	
	MapPlayerKey.MappingName = KeyMap.GetMappingName();
	MapPlayerKey.Slot = EPlayerMappableKeySlot::First;
	MapPlayerKey.NewKey = SelectedKey.Key;
	
	UserSettings->MapPlayerKey(MapPlayerKey, FailureReason);
	UserSettings->ApplySettings();
	UserSettings->SaveSettings();
}

void USettingItem_KeySelector::HandleSelectedInputKeySelector()
{
	bIsSelectedChanged = true;
}

void USettingItem_KeySelector::HandleUpdate(FKey Key)
{
	if (IKey_Selector)
	{
		FInputChord KeyChord;

		KeyChord.Key = Key;
		KeyChord.bCtrl = false;
		KeyChord.bShift = false;
		KeyChord.bAlt = false;
		KeyChord.bCmd = false;
		
		IKey_Selector->SetSelectedKey(KeyChord);
	}
}

void USettingItem_KeySelector::HandleReset()
{
	FMapPlayerKeyArgs MapPlayerKey;
	FGameplayTagContainer FailureReason;
	
	MapPlayerKey.MappingName = KeyMap.GetMappingName();
	MapPlayerKey.Slot = EPlayerMappableKeySlot::First;
	MapPlayerKey.NewKey = KeyMap.GetDefaultKey();
	
	UserSettings->MapPlayerKey(MapPlayerKey, FailureReason);
	UserSettings->ApplySettings();
	UserSettings->SaveSettings();

	HandleUpdate(KeyMap.GetDefaultKey());
}

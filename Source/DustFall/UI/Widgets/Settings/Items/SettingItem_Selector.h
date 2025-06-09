// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SettingItemWidget.h"
#include "SettingItem_Selector.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionChanged, const int32&, Index);

/**
 * 
 */
UCLASS()
class DUSTFALL_API USettingItem_Selector : public USettingItemWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* TextSelector;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonPreviousOption;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonNextOption;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Selector")
	TArray<FString> Options;

	int32 CurrentIndex;

	virtual void NativeConstruct() override;

	virtual void NativePreConstruct() override;

public:
	UPROPERTY(BlueprintAssignable, Category = "Events")
	FOnSelectionChanged OnSelectionChanged;

	UFUNCTION()
	void SetSelectedIndex(const int32 Index);

private:
	UFUNCTION()
	void OnNext();

	UFUNCTION()
	void OnPrevious();

	void UpdateTextSelector(int32 Index);
};

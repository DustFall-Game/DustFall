// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DownloadScreenWidget.generated.h"

class UScrollBox;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UDownloadScreenWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	/** Binds */
	UPROPERTY(meta = (BindWidget))
	UScrollBox* SB_Saves;

	/** References */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UUserWidget> SaveGameSlotWidgetClass;
};

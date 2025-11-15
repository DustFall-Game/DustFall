// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/UI/Widgets/BaseUserWidget/BaseUserWidget.h"
#include "PauseMenuWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UPauseMenuWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

protected:
	/** Binds */
	UPROPERTY(meta = (BindWidget))
	UButton* Btn_ExitGame;

	UFUNCTION()
	void OnExitGameClicked();
};

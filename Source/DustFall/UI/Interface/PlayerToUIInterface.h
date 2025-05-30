// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PlayerToUIInterface.generated.h"

class UDF_BaseUserWidget;
// This class does not need to be modified.
UINTERFACE()
class UPlayerToUIInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class DUSTFALL_API IPlayerToUIInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	UDF_BaseUserWidget* GetUI(FName WidgetName);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	bool ShowUI(TSubclassOf<UDF_BaseUserWidget> WidgetClass);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="Interfaces")
	void HandleEscape();
};

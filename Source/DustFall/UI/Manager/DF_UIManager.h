// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustFall/UI/Interface/PlayerToUIInterface.h"
#include "DustFall/UI/Widgets/BaseUserWidget/DF_BaseUserWidget.h"
#include "DF_UIManager.generated.h"


class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UDF_UIManager : public UActorComponent, public IPlayerToUIInterface
{
	GENERATED_BODY()

public:
	UDF_UIManager();
	
	/** Interfaces */
	UDF_BaseUserWidget* GetUI_Implementation(FName WidgetName) override;
	bool ShowUI_Implementation(TSubclassOf<UDF_BaseUserWidget> WidgetClass) override;
	void HandleEscape_Implementation() override;

protected:
	virtual void BeginPlay() override;
	void SetInputSettings(bool bIsUIActive);
	void ChangeVisibilityWidget(UDF_BaseUserWidget* Widget);
	UDF_BaseUserWidget* GetActivityWidgetByClass(TSubclassOf<UDF_BaseUserWidget> WidgetClass);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UDF_BaseUserWidget> PauseMenuWidget;

	UPROPERTY()
	UDF_BaseUserWidget* ActivityWidget;

	UPROPERTY()
	APlayerController* PlayerController;
	
	UPROPERTY()
	TMap<FName, UDF_BaseUserWidget*> Widgets;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* MainInputMappingContext;
	
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UInputMappingContext* PauseInputMappingContext;
};

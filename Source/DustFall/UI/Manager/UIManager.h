// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "DustFall/UI/Widgets/BaseUserWidget/BaseUserWidget.h"
#include "UIManager.generated.h"


class UInputMappingContext;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UUIManager : public UActorComponent
{
	GENERATED_BODY()

public:
	UUIManager();
	
	UFUNCTION(BlueprintCallable, Category="UI")
	UBaseUserWidget* GetWidgetByName(FName WidgetName);
	
	UFUNCTION(BlueprintCallable, Category="UI")
	bool ShowUI(TSubclassOf<UBaseUserWidget> WidgetClass);
	
	UFUNCTION(BlueprintCallable, Category="UI")
	void HandleEscape();
	
	UFUNCTION(BlueprintCallable, Category="UI")
	void CloseAllUI();

protected:
	virtual void BeginPlay() override;

private:
	UBaseUserWidget* GetOrCreateWidget(TSubclassOf<UBaseUserWidget> WidgetClass);
	void SetInputMode(bool bUIActive);
	
	/** Data */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI", meta=(AllowPrivateAccess="true"))
	TSubclassOf<UBaseUserWidget> PauseMenuWidget;

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* MainInputMappingContext;

	UPROPERTY(EditDefaultsOnly, Category="Input", meta=(AllowPrivateAccess="true"))
	UInputMappingContext* PauseInputMappingContext;
	
	/** References */
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	UBaseUserWidget* ActiveWidget = nullptr;

	UPROPERTY()
	TMap<FName, UBaseUserWidget*> Widgets;
};

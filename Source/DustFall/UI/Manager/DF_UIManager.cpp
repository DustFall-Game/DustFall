// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_UIManager.h"

#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"


UDF_UIManager::UDF_UIManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

UDF_BaseUserWidget* UDF_UIManager::GetUI_Implementation(FName WidgetName)
{
	if (UDF_BaseUserWidget** FoundWidget = Widgets.Find(WidgetName))
	{
		return *FoundWidget;
	}

	return nullptr;
}

void UDF_UIManager::BeginPlay()
{
	Super::BeginPlay();

	if (!GetOwner()) return;
	
	PlayerController = Cast<APlayerController>(GetOwner());
}

bool UDF_UIManager::ShowUI_Implementation(TSubclassOf<UDF_BaseUserWidget> WidgetClass)
{
	if (!WidgetClass) return false;

	UDF_BaseUserWidget* Widget = GetActivityWidgetByClass(WidgetClass);

	if (Widget)
	{
		if (Widget->IsVisible())
		{
			if (Widget->bCanClose)
			{
				Widget->SetVisibility(ESlateVisibility::Collapsed);
				ActivityWidget = nullptr;
				SetInputSettings(false);
				return false;
			}
			return true;
		}
		
		Widget->SetVisibility(ESlateVisibility::Visible);
		if (Widget->bCanClose)
		{
			ActivityWidget = Widget;
		}
		SetInputSettings(true);
		return true;
	}
	
	auto NewWidget = CreateWidget<UDF_BaseUserWidget>(PlayerController, WidgetClass);
	if (!NewWidget) return false;

	NewWidget->AddToViewport();
	Widgets.Add(NewWidget->WidgetName, NewWidget);

	if (NewWidget->bCanClose)
	{
		ActivityWidget = NewWidget;
	}

	SetInputSettings(true);
	return true;
}

void UDF_UIManager::HandleEscape_Implementation()
{
	if (ActivityWidget && ActivityWidget->bCanClose)
	{
		ActivityWidget->Close();
		ActivityWidget->SetVisibility(ESlateVisibility::Collapsed);
		ActivityWidget = nullptr;
		SetInputSettings(false);
		return;
	}
	
	if (!PauseMenuWidget) return;

	UDF_BaseUserWidget* PauseWidget = GetActivityWidgetByClass(PauseMenuWidget);

	if (PauseWidget)
	{
		bool bIsVisible = PauseWidget->IsVisible();
		PauseWidget->SetVisibility(bIsVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
		ActivityWidget = bIsVisible ? nullptr : PauseWidget;
		SetInputSettings(!bIsVisible);
	}
	else
	{
		auto NewPauseWidget = CreateWidget<UDF_BaseUserWidget>(PlayerController, PauseMenuWidget);
		if (NewPauseWidget)
		{
			NewPauseWidget->AddToViewport();
			Widgets.Add(NewPauseWidget->WidgetName, NewPauseWidget);
			ActivityWidget = NewPauseWidget;
			SetInputSettings(true);
		}
	}
}

void UDF_UIManager::ChangeVisibilityWidget(UDF_BaseUserWidget* Widget)
{
	if (!IsValid(Widget)) return;

	bool bVisible = Widget->IsVisible();
	Widget->SetVisibility(bVisible ? ESlateVisibility::Collapsed : ESlateVisibility::Visible);
	SetInputSettings(!bVisible);
}

UDF_BaseUserWidget* UDF_UIManager::GetActivityWidgetByClass(TSubclassOf<UDF_BaseUserWidget> WidgetClass)
{
	for (const TPair<FName, UDF_BaseUserWidget*>& Pair : Widgets)
	{
		UDF_BaseUserWidget* Widget = Pair.Value;
		if (IsValid(Widget) && Widget->GetClass() == WidgetClass)
		{
			return Widget;
		}
	}
	return nullptr;
}

void UDF_UIManager::SetInputSettings(bool bIsUIActive)
{
	if (!PlayerController) return;

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (MainInputMappingContext && PauseInputMappingContext)
			{
				Subsystem->RemoveMappingContext(bIsUIActive ? MainInputMappingContext : PauseInputMappingContext);
				Subsystem->AddMappingContext(bIsUIActive ? PauseInputMappingContext : MainInputMappingContext, 0);
			}
		}
		PlayerController->bShowMouseCursor = bIsUIActive;
	}
}

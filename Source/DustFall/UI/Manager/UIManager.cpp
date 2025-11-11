// Fill out your copyright notice in the Description page of Project Settings.


#include "UIManager.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"


UUIManager::UUIManager()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUIManager::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = Cast<APlayerController>(GetOwner());
	if (!PlayerController)
		UE_LOG(LogTemp, Warning, TEXT("UUIManager: Owner is not PlayerController."));
}

UBaseUserWidget* UUIManager::GetWidgetByName(FName WidgetName)
{
	if (UBaseUserWidget* const* Found = Widgets.Find(WidgetName))
		return *Found;
	
	return nullptr;
}

bool UUIManager::ShowUI(TSubclassOf<UBaseUserWidget> WidgetClass)
{
	if (!WidgetClass || !PlayerController) return false;

	auto Widget = GetOrCreateWidget(WidgetClass);
	if (!Widget) return false;
	
	if (Widget->IsVisible())
	{
		if (Widget->bCanClose)
		{
			Widget->SetVisibility(ESlateVisibility::Collapsed);
			
			if (ActiveWidget == Widget)
				ActiveWidget = nullptr;
			
			SetInputMode(false);
		}
		
		return false;
	}
	
	if (ActiveWidget && ActiveWidget->bCanClose)
		ActiveWidget->SetVisibility(ESlateVisibility::Collapsed);
	
	Widget->SetVisibility(ESlateVisibility::Visible);
	if (Widget->bCanClose)
		ActiveWidget = Widget;
	
	SetInputMode(true);
	return true;
}

void UUIManager::HandleEscape()
{
	if (ActiveWidget && ActiveWidget->bCanClose)
	{
		ActiveWidget->Close();
		ActiveWidget->SetVisibility(ESlateVisibility::Collapsed);
		ActiveWidget = nullptr;
		SetInputMode(false);
		
		return;
	}
	
	if (PauseMenuWidget)
		ShowUI(PauseMenuWidget);
}

void UUIManager::CloseAllUI()
{
	for (auto& Pair : Widgets)
	{
		if (UBaseUserWidget* Widget = Pair.Value)
			if (Widget->IsVisible())
				Widget->SetVisibility(ESlateVisibility::Collapsed);
	}

	ActiveWidget = nullptr;
	SetInputMode(false);
}

UBaseUserWidget* UUIManager::GetOrCreateWidget(TSubclassOf<UBaseUserWidget> WidgetClass)
{
	for (auto& Pair : Widgets)
	{
		if (Pair.Value && Pair.Value->GetClass() == WidgetClass)
			return Pair.Value;
	}
	
	auto NewWidget = CreateWidget<UBaseUserWidget>(PlayerController, WidgetClass);
	if (!NewWidget) return nullptr;

	NewWidget->AddToViewport();
	Widgets.Add(NewWidget->WidgetName, NewWidget);
	
	return NewWidget;
}

void UUIManager::SetInputMode(bool bUIActive)
{
	if (!PlayerController) return;

	if (ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer())
		if (auto Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
			if (MainInputMappingContext && PauseInputMappingContext)
			{
				Subsystem->RemoveMappingContext(bUIActive ? MainInputMappingContext : PauseInputMappingContext);
				Subsystem->AddMappingContext(bUIActive ? PauseInputMappingContext : MainInputMappingContext, 0);
			}

	PlayerController->bShowMouseCursor = bUIActive;

	/*
	if (bUIActive)
		PlayerController->SetInputMode(FInputModeUIOnly());
	else
		PlayerController->SetInputMode(FInputModeGameOnly());
	*/
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "Components/Button.h"
#include "DustFall/Systems/SaveGameManager/SaveGameManager.h"
#include "Kismet/KismetSystemLibrary.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (Btn_ExitGame)
	{
		Btn_ExitGame->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnExitGameClicked);
	}
}

void UPauseMenuWidget::OnExitGameClicked()
{
	UWorld* World = GetWorld();
	if (!World) return;
	
	if (World->GetAuthGameMode())
		USaveGameManager::SaveGame(GetWorld());

	if (auto PC= GetOwningPlayer())
		UKismetSystemLibrary::QuitGame(World, PC, EQuitPreference::Quit, true);
}

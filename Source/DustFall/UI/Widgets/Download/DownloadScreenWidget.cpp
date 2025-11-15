// Fill out your copyright notice in the Description page of Project Settings.


#include "DownloadScreenWidget.h"
#include "Components/Image.h"
#include "Components/ScrollBox.h"
#include "Components/TextBlock.h"
#include "DustFall/Systems/SaveGameManager/SaveGameManager.h"
#include "DustFall/UI/Widgets/SaveGameSlot/SaveGameSlotWidget.h"

void UDownloadScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!SB_Saves || !SaveGameSlotWidgetClass) return;

	TArray<FSaveGameInfo> SaveList = USaveGameManager::GetAllSaveGames();
	for (auto SaveInfo : SaveList)
	{
		auto Widget = CreateWidget<USaveGameSlotWidget>(this, SaveGameSlotWidgetClass);
		if (!Widget) continue;

		FString BaseName;
		FString Timestamp;

		if (SaveInfo.SaveName.Split(TEXT("-"), &BaseName, &Timestamp))
		{
			Widget->Text_SaveName->SetText(FText::FromString(BaseName));
			Widget->Text_SaveTimestamp->SetText(FText::FromString(Timestamp));
		} else
			Widget->Text_SaveName->SetText(FText::FromString(SaveInfo.SaveName));
		
		Widget->Text_SaveData->SetText(FText::FromString(SaveInfo.SaveDate.ToString(TEXT("%Y-%m-%d %H:%M:%S"))));

		if (SaveInfo.SaveScreenshot)
		{
			FSlateBrush Brush;
			Brush.SetResourceObject(SaveInfo.SaveScreenshot);
			Brush.ImageSize = FVector2D(512.f, 190.f);
			
			Widget->Img_SaveScreenshot->SetBrush(Brush);			
		}

		SB_Saves->AddChild(Widget);
	}
}

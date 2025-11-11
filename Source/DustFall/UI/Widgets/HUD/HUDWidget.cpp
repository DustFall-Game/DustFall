// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "DustFall/Core/Player/Components/Ability/PlayerAbilityComponent.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto PA = GetOwningPlayerPawn()->FindComponentByClass<UPlayerAbilityComponent>()) {
		PA->OnStatChanged.AddDynamic(this, &UHUDWidget::HandleStatChanged);

		if (!Text_Bleeding && !PB_Health && !PB_Satiety && !PB_Thirst) return;
		
		PB_Health->SetPercent(PA->GetHealth() / 100.f);
		PB_Satiety->SetPercent(PA->GetSatiety() / 100.f);
		PB_Thirst->SetPercent(PA->GetThirst() / 100.f);
	}
}

void UHUDWidget::HandleStatChanged(FName StatName, float NewValue)
{
	if (!Text_Bleeding && !PB_Health && !PB_Satiety && !PB_Thirst && !PB_Stamina) return;
	
	const float NewValuePercent = NewValue / 100.f;
	
	if (StatName == "Health")
		PB_Health->SetPercent(NewValuePercent);
	if (StatName == "Satiety")
		PB_Satiety->SetPercent(NewValuePercent);
	if (StatName == "Thirst")
		PB_Thirst->SetPercent(NewValuePercent);
	if (StatName == "Stamina")
		PB_Stamina->SetPercent(NewValuePercent);
	if (StatName == "Bleeding")
		Text_Bleeding->SetText(FText::FromString(FString::FromInt(NewValue)));
}

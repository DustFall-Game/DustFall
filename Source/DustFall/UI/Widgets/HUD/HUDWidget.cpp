// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDWidget.h"
#include "DustFall/Core/Player/Components/Ability/PlayerAbilityComponent.h"

void UHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (auto PA = GetOwningPlayerPawn()->FindComponentByClass<UPlayerAbilityComponent>())
		PA->OnStatChanged.AddDynamic(this, &UHUDWidget::HandleStatChanged);
}

void UHUDWidget::HandleStatChanged(FName StatName, float NewValue)
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"


UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UAbilityComponent::HandleSprint_Implementation(bool bIsSprint)
{
	
}

void UAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

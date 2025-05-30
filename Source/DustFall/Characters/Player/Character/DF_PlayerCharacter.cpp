// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_PlayerCharacter.h"

#include "DustFall/Characters/Player/Components/Ability/DF_AbilityComponent.h"


ADF_PlayerCharacter::ADF_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;

	AbilityComponent = FindComponentByClass<UDF_AbilityComponent>();
}

void ADF_PlayerCharacter::HandleCrouch_Implementation(bool bIsNewCrouch)
{
	if (bIsNewCrouch)
	{
		Crouch();
	} else
	{
		UnCrouch();
	}
}

void ADF_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
}

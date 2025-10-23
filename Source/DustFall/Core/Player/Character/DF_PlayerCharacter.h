// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/Core/Player/Interfaces/InputToPlayerInterface.h"
#include "GameFramework/Character.h"
#include "DF_PlayerCharacter.generated.h"

class UPlayerAbilityComponent;

UCLASS()
class DUSTFALL_API ADF_PlayerCharacter : public ACharacter, public IInputToPlayerInterface
{
	GENERATED_BODY()

public:
	ADF_PlayerCharacter();

	virtual void HandleCrouch_Implementation(bool bIsNewCrouch) override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY()
	UPlayerAbilityComponent* AbilityComponent;
};

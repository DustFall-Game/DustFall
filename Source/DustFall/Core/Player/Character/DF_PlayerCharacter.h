﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DustFall/Core/Player/Interfaces/InputToPlayerInterface.h"
#include "GameFramework/Character.h"
#include "DF_PlayerCharacter.generated.h"

class UBaseUserWidget;
class UPlayerAbilityComponent;

UCLASS()
class DUSTFALL_API ADF_PlayerCharacter : public ACharacter, public IInputToPlayerInterface
{
	GENERATED_BODY()

public:
	ADF_PlayerCharacter();

	/** Implements */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;
	virtual void HandleCrouch_Implementation(bool bIsNewCrouch) override;

	UFUNCTION(Server, Reliable)
	void ServerHandleSprint(bool bIsNewSprint);

protected:
	virtual void BeginPlay() override;

	/** Handlers */
	void HandleDrainStamina();
	void HandleRegenStamina();

	/** References */
	UPROPERTY()
	UPlayerAbilityComponent* AbilityComponent;

	UPROPERTY()
	UCharacterMovementComponent* MovementComponent;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="UI")
	TSubclassOf<UBaseUserWidget> HUDWidgetClass;

private:
	/** Timers */
	FTimerHandle StaminaDrainTimerHandle;
	FTimerHandle StaminaRegenTimerHandle;
};

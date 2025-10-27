// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_PlayerCharacter.h"

#include "Blueprint/UserWidget.h"
#include "DustFall/Core/Player/Components/Ability/PlayerAbilityComponent.h"
#include "DustFall/UI/Manager/UIManager.h"
#include "GameFramework/CharacterMovementComponent.h"


ADF_PlayerCharacter::ADF_PlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
}

void ADF_PlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	AbilityComponent = FindComponentByClass<UPlayerAbilityComponent>();
	PlayerController = Cast<APlayerController>(GetController());

	if (HUDWidgetClass && PlayerController)
		if (auto UIManager = PlayerController->FindComponentByClass<UUIManager>())
			UIManager->ShowUI(HUDWidgetClass);
}

void ADF_PlayerCharacter::HandleSprint_Implementation(bool bIsSprint)
{
	if (!AbilityComponent) return;
	
	if (MovementComponent = GetCharacterMovement(); MovementComponent)
	{
		if (bIsSprint && (AbilityComponent->GetStamina() < 15.f || MovementComponent->Velocity.Size2D() < 10.0f))
			return;

		AbilityComponent->SetIsSprinting(bIsSprint);
		MovementComponent->MaxWalkSpeed = bIsSprint ? 600.f : 300.f;

		auto& TimerManager = GetWorld()->GetTimerManager();

		if (bIsSprint)
		{
			TimerManager.ClearTimer(StaminaRegenTimerHandle);

			if (!TimerManager.IsTimerActive(StaminaDrainTimerHandle))
				TimerManager.SetTimer(
					StaminaDrainTimerHandle,
					this,
					&ADF_PlayerCharacter::HandleDrainStamina,
					0.1f,
					true
				);
		} else {
			TimerManager.ClearTimer(StaminaDrainTimerHandle);

			if (!TimerManager.IsTimerActive(StaminaRegenTimerHandle))
				TimerManager.SetTimer(
					StaminaRegenTimerHandle,
					this,
					&ADF_PlayerCharacter::HandleRegenStamina,
					0.1f,
					true
				);
		}
	}
}

void ADF_PlayerCharacter::HandleDrainStamina()
{
	if (!AbilityComponent || !MovementComponent) return;

	float NewStamina = FMath::Max(AbilityComponent->GetStamina() - 0.9f, 0.f);
	AbilityComponent->SetStamina(NewStamina);

	if (FMath::IsNearlyZero(NewStamina) || MovementComponent->Velocity.Size2D() < 10.0f)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDrainTimerHandle);
		Execute_HandleSprint(this, false);
	}
}

void ADF_PlayerCharacter::HandleRegenStamina()
{
	if (!AbilityComponent) return;
	
	float NewStamina = FMath::Min(AbilityComponent->GetStamina() + 0.6f, AbilityComponent->GetMaxStamina());
	AbilityComponent->SetStamina(NewStamina);

	if (FMath::IsNearlyEqual(NewStamina, AbilityComponent->GetMaxStamina()))
		GetWorld()->GetTimerManager().ClearTimer(StaminaRegenTimerHandle);
}

void ADF_PlayerCharacter::HandleCrouch_Implementation(bool bIsNewCrouch)
{
	if (bIsNewCrouch)
		Crouch();
	else
		UnCrouch();
}

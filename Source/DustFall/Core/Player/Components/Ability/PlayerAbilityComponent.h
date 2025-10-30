// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustFall/Core/Components/Ability/BaseAbilityComponent.h"
#include "PlayerAbilityComponent.generated.h"

class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FName, StatName, float, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UPlayerAbilityComponent : public UBaseAbilityComponent
{
	GENERATED_BODY()

public:
	/** Getters */
	float GetHealth() { return Health; }
	float GetStamina() { return Stamina; }
	float GetMaxStamina() { return MaxStamina; }
	float GetSatiety() { return Satiety; }
	float GetThirst() { return Thirst; }

	/** Setters */
	void SetIsSprinting(bool NewSprint) { bIsSprinting = NewSprint; }
	void SetStamina(float NewStamina);

	/** Condition coefficients */
	float SprintMultiplier = 1.7f;
	float IdleMultiplier = 1.0f;
	
	/** Client RPCs */
	UFUNCTION(Client, Reliable)
	void Client_Die();
	
	/** Implements */
	virtual void TakeDamage_Implementation(float Damage, AActor* Character, FName Bone) override;

	/** Delegats */
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;

protected:
	virtual void BeginPlay() override;
	virtual void OnRep_Health() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** Setters*/
	void SetHealth(float NewHealth);
	void SetSatiety(float NewSatiety);
	void SetThirst(float NewThirst);
	void SetBleeding(float NewBleeding);

	/** References */
	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeadScreenWidgetClass;

	/** Handlers */
	UFUNCTION()
	void HandleRegenerateHealth();

	UFUNCTION()
	void HandleBleeding();

	UFUNCTION()
	void HandleHunger();

	UFUNCTION()
	void HandleThirst();

private:
	/** Timers */
	FTimerHandle RegenTimerHandle;
	FTimerHandle BleedingTimerHandle;
	FTimerHandle HungerTimerHandle;
	FTimerHandle ThirstTimerHandle;

	float BaseHungerRate = 0.1f;
	float BaseThirstRate = 0.12f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxSatiety = 95.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxThirst = 90.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Satiety;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Thirst;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Bleeding;

	UPROPERTY(Replicated, BlueprintReadOnly, Category="State", meta = (AllowPrivateAccess = "true"))
	bool bIsSprinting = false;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustFall/Core/Player/Interfaces/InputToPlayerInterface.h"
#include "DustFall/Core/Interfaces/DamageInterface.h"
#include "PlayerAbilityComponent.generated.h"


class UCharacterMovementComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnStatChanged, FName, StatName, float, NewValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UPlayerAbilityComponent : public UActorComponent, public IInputToPlayerInterface, public IDamageInterface
{
	GENERATED_BODY()

public:
	/** Client RPCs */
	UFUNCTION(Client, Reliable)
	void Client_PlayerDie();

	/** Server RPCs */
	UFUNCTION(Server, Reliable)
	void Server_PlayerDie();

	/** Multicast RPCs */
	UFUNCTION(NetMulticast, Reliable)
	void Multi_PlayerDie();

	/** Implements */
	virtual void HandleSprint_Implementation(bool bIsSprint) override;
	virtual void TakeDamage_Implementation(float Damage, AActor* Character, FName Bone) override;

	/** Delegats **/
	UPROPERTY(BlueprintAssignable)
	FOnStatChanged OnStatChanged;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** References */
	UPROPERTY()
	ACharacter* PlayerCharacter = nullptr;

	UPROPERTY()
	APlayerController* PlayerController = nullptr;

	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeadScreenWidgetClass;

	UFUNCTION()
	void HandleRegenerateHealth();

	UFUNCTION()
	void HandleBleeding();

	UFUNCTION()
	void OnRep_Health();

	UFUNCTION()
	void OnRep_Stamina();

	UFUNCTION()
	void OnRep_Satiety();

	UFUNCTION()
	void OnRep_Thirst();

	UFUNCTION()
	void OnRep_Bleeding();

private:
	/** Timer Handles */
	FTimerHandle RegenTimerHandle;
	FTimerHandle BleedingTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxHealth = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxStamina = 100.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxSatiety = 60.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float MaxThirst = 70.f;
	
	UPROPERTY(ReplicatedUsing=OnRep_Health, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Health;

	UPROPERTY(ReplicatedUsing=OnRep_Stamina, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Stamina;

	UPROPERTY(ReplicatedUsing=OnRep_Satiety, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Satiety;

	UPROPERTY(ReplicatedUsing=OnRep_Thirst, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Thirst;

	UPROPERTY(ReplicatedUsing=OnRep_Bleeding, BlueprintReadOnly, Category = "PlayerAbility", meta = (AllowPrivateAccess = "true"))
	float Bleeding;
};

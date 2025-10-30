// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "DustFall/Core/Interfaces/DamageInterface.h"
#include "BaseAbilityComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DUSTFALL_API UBaseAbilityComponent : public UActorComponent, public IDamageInterface
{
	GENERATED_BODY()

public:
	UBaseAbilityComponent();
	
	/** Implements */
	virtual void TakeDamage_Implementation(float Damage, AActor* Character, FName Bone) override;

protected:
	virtual void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	/** Server RPCs */
	UFUNCTION(Server, Reliable)
	virtual void Server_Die();

	/** Multicast RPCs */
	UFUNCTION(NetMulticast, Reliable)
	virtual void Multi_Die();

	/** References */
	UPROPERTY()
	ACharacter* CharacterRef;

	/** OnReps */
	UFUNCTION()
	virtual void OnRep_Health() {};
	
	UPROPERTY(ReplicatedUsing=OnRep_Health)
	float Health;
	
	float MaxHealth = 100.f;
};

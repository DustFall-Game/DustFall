// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAbilityComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Damage.h"


UBaseAbilityComponent::UBaseAbilityComponent()
{
	SetIsReplicatedByDefault(true);
}

void UBaseAbilityComponent::BeginPlay()
{
	Super::BeginPlay();
	
	Health = MaxHealth;
	CharacterRef = Cast<ACharacter>(GetOwner());
}

void UBaseAbilityComponent::TakeDamage_Implementation(float Damage, AActor* Character, FName Bone)
{
	Health -= Damage;

	UE_LOG(LogTemp, Warning, TEXT("Health = %f"), Health);

	if (Health <= 0)
		Server_Die();

	OnDamageTaken.Broadcast();
}

void UBaseAbilityComponent::Server_Die_Implementation()
{
	Multi_Die();
}

void UBaseAbilityComponent::Multi_Die_Implementation()
{
	if (!CharacterRef) return;
	
	if (auto CapsuleComponent = CharacterRef->GetComponentByClass<UCapsuleComponent>())
		CapsuleComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (auto SkeletalMeshComponent = CharacterRef->GetComponentByClass<USkeletalMeshComponent>())
	{
		SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkeletalMeshComponent->SetSimulatePhysics(true);
	}

	if (auto MovementComponent = CharacterRef->FindComponentByClass<UCharacterMovementComponent>())
		MovementComponent->DisableMovement();
}

void UBaseAbilityComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseAbilityComponent, Health);
}

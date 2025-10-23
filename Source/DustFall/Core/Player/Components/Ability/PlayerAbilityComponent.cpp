// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAbilityComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UPlayerAbilityComponent::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
	Stamina = MaxStamina;
	Satiety = MaxSatiety;
	Thirst = MaxThirst;

	GetWorld()->GetTimerManager().SetTimer(
		RegenTimerHandle,
		this,
		&UPlayerAbilityComponent::HandleRegenerateHealth,
		1.0f,
		true
	);
		
	PlayerCharacter = Cast<ACharacter>(GetOwner());
	PlayerController = Cast<APlayerController>(PlayerCharacter->GetController());
	
	CharacterMovement = PlayerCharacter->GetComponentByClass<UCharacterMovementComponent>();
}

void UPlayerAbilityComponent::Client_PlayerDie_Implementation()
{
	if (PlayerController && PlayerController->IsPlayerController())
	{
		Server_PlayerDie();
		
		if (DeadScreenWidgetClass)
			if (UUserWidget* DeadScreenWidget = CreateWidget<UUserWidget>(GetWorld(), DeadScreenWidgetClass))
				DeadScreenWidget->AddToViewport();
	}
}

void UPlayerAbilityComponent::Server_PlayerDie_Implementation()
{
	Multi_PlayerDie();
}

void UPlayerAbilityComponent::Multi_PlayerDie_Implementation()
{
	if (!CharacterMovement) return;

	if (UCapsuleComponent* Capsule = PlayerCharacter->GetComponentByClass<UCapsuleComponent>())
		Capsule->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	if (USkeletalMeshComponent* SkeletalMeshComponent = PlayerCharacter->GetComponentByClass<USkeletalMeshComponent>())
	{
		SkeletalMeshComponent->SetCollisionObjectType(ECollisionChannel::ECC_PhysicsBody);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
		SkeletalMeshComponent->SetSimulatePhysics(true);
	}
	
	if (CharacterMovement)
		CharacterMovement->DisableMovement();
}

void UPlayerAbilityComponent::HandleRegenerateHealth()
{
	if (Bleeding <= 0)
		if (Satiety > MaxSatiety * 0.6f && Health < MaxHealth)
			Health = FMath::Min(Health + 0.1f, MaxHealth);
}

void UPlayerAbilityComponent::HandleBleeding()
{
	Health -= 1.f;
	Bleeding -= 1;
	
	if (Health <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
		Client_PlayerDie();
		return;
	}
	
	if (Bleeding <= 0)
		GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
}

void UPlayerAbilityComponent::HandleSprint_Implementation(bool bIsSprint)
{
	
}

void UPlayerAbilityComponent::TakeDamage_Implementation(float Damage, AActor* Character, FName Bone)
{
	Health -= Damage;

	if (Health <= 0)
	{
		Client_PlayerDie();
		return;
	}

	if (FMath::FRand() <= 0.4f) {
		Bleeding = FMath::RandRange(2.f, 4.f);
		
		GetWorld()->GetTimerManager().SetTimer(BleedingTimerHandle, this, &UPlayerAbilityComponent::HandleBleeding, 1.5f, true);
	}
}

void UPlayerAbilityComponent::OnRep_Health()
{
	OnStatChanged.Broadcast("Health", Health);
}

void UPlayerAbilityComponent::OnRep_Stamina()
{
	OnStatChanged.Broadcast("Stamina", Stamina);
}

void UPlayerAbilityComponent::OnRep_Satiety()
{
	OnStatChanged.Broadcast("Satiety", Satiety);
}

void UPlayerAbilityComponent::OnRep_Thirst()
{
	OnStatChanged.Broadcast("Thirst", Thirst);
}

void UPlayerAbilityComponent::OnRep_Bleeding()
{
	OnStatChanged.Broadcast("Bleeding", Bleeding);
}

void UPlayerAbilityComponent::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UPlayerAbilityComponent, Health);
	DOREPLIFETIME(UPlayerAbilityComponent, Stamina);
	DOREPLIFETIME(UPlayerAbilityComponent, Satiety);
	DOREPLIFETIME(UPlayerAbilityComponent, Thirst);
	DOREPLIFETIME(UPlayerAbilityComponent, Bleeding);
}

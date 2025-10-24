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
		1.f,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		HungerTimerHandle,
		this,
		&UPlayerAbilityComponent::HandleHunger,
		1.f,
		true
	);

	GetWorld()->GetTimerManager().SetTimer(
		ThirstTimerHandle,
		this,
		&UPlayerAbilityComponent::HandleThirst,
		1.f,
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
		if (Satiety > MaxSatiety * 0.6f && Satiety >= 25.f && Health < MaxHealth)
			SetHealth(FMath::Min(Health + 0.1f, MaxHealth));
}

void UPlayerAbilityComponent::HandleBleeding()
{
	SetHealth(Health - 1.f);
	SetBleeding(Bleeding - 1);
	
	if (Health <= 0.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
		Client_PlayerDie();
		return;
	}
	
	if (Bleeding <= 0)
		GetWorld()->GetTimerManager().ClearTimer(BleedingTimerHandle);
}

void UPlayerAbilityComponent::HandleHunger()
{
	float Multiplier = bIsSprinting ? SprintMultiplier : IdleMultiplier;
	SetSatiety(FMath::Clamp(Satiety - BaseHungerRate * Multiplier, 0.f, MaxSatiety));
}

void UPlayerAbilityComponent::HandleThirst()
{
	float Multiplier = bIsSprinting ? SprintMultiplier : IdleMultiplier;
	SetThirst(FMath::Clamp(Thirst - BaseThirstRate * Multiplier, 0.f, MaxThirst));

	MaxStamina = Thirst <= 20.f ? 70.f : 100.f;
}

void UPlayerAbilityComponent::TakeDamage_Implementation(float Damage, AActor* Character, FName Bone)
{
	SetHealth(Health - Damage);

	if (Health <= 0)
	{
		Client_PlayerDie();
		return;
	}

	if (FMath::FRand() <= 0.4f) {
		SetBleeding(Bleeding + FMath::RandRange(1.f, 3.f));
		
		GetWorld()->GetTimerManager().SetTimer(BleedingTimerHandle, this, &UPlayerAbilityComponent::HandleBleeding, 2.f, true);
	}
}

void UPlayerAbilityComponent::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.f, MaxHealth);
	OnStatChanged.Broadcast("Health", Health);
}

void UPlayerAbilityComponent::SetStamina(float NewStamina)
{
	Stamina = FMath::Clamp(NewStamina, 0.f, MaxStamina);
	OnStatChanged.Broadcast("Stamina", Stamina);
}

void UPlayerAbilityComponent::SetSatiety(float NewSatiety)
{
	Satiety = FMath::Clamp(NewSatiety, 0.f, MaxSatiety);
	OnStatChanged.Broadcast("Satiety", Satiety);
}

void UPlayerAbilityComponent::SetThirst(float NewThirst)
{
	Thirst = FMath::Clamp(NewThirst, 0.f, MaxThirst);
	OnStatChanged.Broadcast("Thirst", Thirst);
}

void UPlayerAbilityComponent::SetBleeding(float NewBleeding)
{
	Bleeding = FMath::Max(NewBleeding, 0.f);
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
	DOREPLIFETIME(UPlayerAbilityComponent, bIsSprinting);
}

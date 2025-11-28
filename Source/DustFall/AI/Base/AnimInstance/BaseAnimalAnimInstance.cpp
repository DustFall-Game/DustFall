// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseAnimalAnimInstance.h"
#include "AIController.h"
#include "KismetAnimationLibrary.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"

void UBaseAnimalAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Character = Cast<ACharacter>(TryGetPawnOwner());
}

void UBaseAnimalAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Character) return;
	if (!Blackboard)
	{
		auto AIController = Cast<AAIController>(Character->GetController());
		if (!AIController) return;
		
		Blackboard = AIController->GetBlackboardComponent();
		MaxSprintSpeed = Character->GetCharacterMovement()->MaxWalkSpeed;
	}
	
	const FVector Velocity = Character->GetVelocity();
	const FRotator Rotation = Character->GetControlRotation();

	Speed = Velocity.Length();
	Direction = UKismetAnimationLibrary::CalculateDirection(Velocity, Rotation);
	
	uint8 StateValue = Blackboard->GetValueAsEnum("AnimalState");
	AnimalState = static_cast<EAnimalState>(StateValue);
}

void UBaseAnimalAnimInstance::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UBaseAnimalAnimInstance, AnimalState);
}
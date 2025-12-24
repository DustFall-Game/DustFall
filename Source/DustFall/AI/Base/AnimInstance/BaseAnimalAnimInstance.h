// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DustFall/AI/Enums/AnimalState.h"
#include "BaseAnimalAnimInstance.generated.h"

class UBlackboardComponent;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UBaseAnimalAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void InitializeBlackboard(UBlackboardComponent* BB) { Blackboard = BB; };

protected:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Speed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxSprintSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Direction;
	
	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	EAnimalState AnimalState;

private:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY()
	ACharacter* Character;

	UPROPERTY()
	UBlackboardComponent* Blackboard;
};

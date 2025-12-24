// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "DustFall/Core/Interfaces/AttackerInterface.h"
#include "AttackDamage.generated.h"

/**
 * 
 */
UCLASS()
class DUSTFALL_API UAttackDamage : public UAnimNotify
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName DamageBone;
	
	virtual FString GetNotifyName_Implementation() const override { return TEXT("Do Melee Attack Damage"); };
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override
	{
		if (!MeshComp || !DamageBone.IsValid()) return;
		
		AActor* Owner = MeshComp->GetOwner();
		if (!Owner) return;

		if (!Owner->GetClass()->ImplementsInterface(UAttackerInterface::StaticClass()))
			return;

		IAttackerInterface::Execute_DoAttackTrace(Owner, DamageBone);
	};
};

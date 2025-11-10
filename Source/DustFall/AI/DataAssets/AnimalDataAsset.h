#pragma once

#include "CoreMinimal.h"
#include "DustFall/AI/Enums/TeamType.h"
#include "Engine/DataAsset.h"
#include "AnimalDataAsset.generated.h"


USTRUCT(BlueprintType)
struct FTeamAttitudeRule
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ETeamType TargetTeam = ETeamType::None;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TEnumAsByte<ETeamAttitude::Type> Attitude = ETeamAttitude::Neutral;
};

UCLASS(BlueprintType)
class DUSTFALL_API UAnimalDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	/** Моя команда */
	UPROPERTY(EditAnywhere, Category="AI")
	ETeamType TeamType = ETeamType::None;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float WalkSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float SprintSpeed = 450.f;

	/** Реакция на зрение (видит цель — alert) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	bool bReactToSight = true;

	/** Реакция на звук (услышал шум — alert) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	bool bReactToHearing = true;

	/** Реакция на урон (получил удар — flee) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	bool bReactToDamage = true;

	/** Реакция на урон (получил удар — attack) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception")
	bool bAttackOnDamage = true;

	/** Максимальная зона где может увидеть */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception Sight")
	float SightRadius = 2000.f;

	/** Максимальная зона до куда может видеть */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception Sight")
	float LoseSightRadius = 2200.f;

	/** Периферическое зрение */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Perception Sight")
	float PeripheralVision = 110.f;
};

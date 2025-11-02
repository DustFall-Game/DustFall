#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AnimalDataAsset.generated.h"

UCLASS(BlueprintType)
class DUSTFALL_API UAnimalDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float WalkSpeed = 100.f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement")
	float SprintSpeed = 450.f;
};

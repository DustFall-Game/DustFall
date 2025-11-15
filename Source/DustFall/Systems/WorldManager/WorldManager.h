// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/SunMoonDaySequenceActor.h"
#include "GameFramework/Actor.h"
#include "WorldManager.generated.h"

UENUM(BlueprintType)
enum class EWeatherType : uint8
{
	Sunny,
	Cloudy,
	Rain,
	Fog
};

USTRUCT(BlueprintType)
struct FWorldState
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	float DayTime;

	UPROPERTY(BlueprintReadWrite)
	EWeatherType CurrentWeather;

	UPROPERTY(BlueprintReadWrite)
	float CurrentWeatherDuration;

	UPROPERTY(BlueprintReadWrite)
	float CurrentWeatherTargetDuration;
};

UCLASS()
class DUSTFALL_API AWorldManager : public AActor
{
	GENERATED_BODY()

public:
	AWorldManager();
	
	/** Function for obtaining the state of the world */
	virtual FWorldState GetWorldState() const;

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetWeather(EWeatherType NewWeather);
	virtual EWeatherType GetNextWeather();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day/Night")
	ASunMoonDaySequenceActor* DaySequenceActor;

private:
	float CurrentWeatherDuration;
	float CurrentWeatherTargetDuration;
	float MinWeatherDuration;
	float MaxWeatherDuration;
	EWeatherType CurrentWeather;
};

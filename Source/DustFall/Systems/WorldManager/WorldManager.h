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

UCLASS()
class DUSTFALL_API AWorldManager : public AActor
{
	GENERATED_BODY()

public:
	AWorldManager();

protected:
	virtual void OnConstruction(const FTransform& Transform) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetWeather(EWeatherType NewWeather);
	virtual EWeatherType GetNextWeather();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Day/Night")
	ASunMoonDaySequenceActor* DaySequenceActor;

private:
	float WeatherTimer;
	float CurrentWeatherDuration;
	float CurrentWeatherTargetDuration;
	float MinWeatherDuration;
	float MaxWeatherDuration;
	EWeatherType CurrentWeather;
};

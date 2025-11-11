// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldManager.h"
#include "EngineUtils.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	CurrentWeather = EWeatherType::Sunny;
	
	WeatherTimer = 0.f;
	CurrentWeatherDuration = 0.f;
	CurrentWeatherTargetDuration = 0.f;
	MinWeatherDuration = 6.f;  // 10m = 600.f
	MaxWeatherDuration = 24.f; // 40m = 2400.f
}

void AWorldManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (!DaySequenceActor)
		for (TActorIterator<ASunMoonDaySequenceActor> It(GetWorld()); It;)
		{
			DaySequenceActor = *It;
			break;
		}
	
	if (!DaySequenceActor)
		DaySequenceActor = GetWorld()->SpawnActor<ASunMoonDaySequenceActor>(
			ASunMoonDaySequenceActor::StaticClass(),
			GetActorLocation(),
			FRotator::ZeroRotator);

	if (DaySequenceActor)
	{
		DaySequenceActor->SetDayLength(24.f);
		DaySequenceActor->SetTimePerCycle(0.66f); // 40m / 60m
		DaySequenceActor->SetInitialTimeOfDay(8.f);
		DaySequenceActor->SetRunDayCycle(true);
		DaySequenceActor->Play();
	}
}


void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeatherTargetDuration = FMath::RandRange(MinWeatherDuration, MaxWeatherDuration);
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	WeatherTimer += DeltaTime;
	CurrentWeatherDuration += DeltaTime;
	
	if (CurrentWeatherDuration >= CurrentWeatherTargetDuration)
	{
		SetWeather(GetNextWeather());
		
		CurrentWeatherDuration = 0.f;
		CurrentWeatherTargetDuration = FMath::RandRange(MinWeatherDuration, MaxWeatherDuration);
	}
}

void AWorldManager::SetWeather(EWeatherType NewWeather)
{
	if (NewWeather == CurrentWeather) return;

	CurrentWeather = NewWeather;
}

EWeatherType AWorldManager::GetNextWeather()
{
	TArray<EWeatherType> AllWeathers = { EWeatherType::Sunny, EWeatherType::Rain, EWeatherType::Cloudy, EWeatherType::Fog };
	AllWeathers.Remove(CurrentWeather);

	int32 Index = FMath::RandRange(0, AllWeathers.Num() - 1);
	return AllWeathers[Index];
}

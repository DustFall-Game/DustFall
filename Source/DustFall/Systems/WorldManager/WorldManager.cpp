// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldManager.h"
#include "EngineUtils.h"
#include "DustFall/Systems/SaveGameManager/SaveGameManager.h"

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;

	CurrentWeather = EWeatherType::Sunny;
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
		DaySequenceActor->SetTimePerCycle(0.66f); // 40m in Real Life / 1h(60m)
		DaySequenceActor->SetInitialTimeOfDay(8.f);
		DaySequenceActor->SetRunDayCycle(true);
	}
}


void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	CurrentWeatherTargetDuration = FMath::RandRange(MinWeatherDuration, MaxWeatherDuration);
	
	if (auto SaveGame = USaveGameManager::GetSaveGame(GetWorld()))
	{
		CurrentWeather = SaveGame->CurrentWeather;
		CurrentWeatherDuration = SaveGame->CurrentWeatherDuration;
		CurrentWeatherTargetDuration = SaveGame->CurrentWeatherTargetDuration;
		
		DaySequenceActor->SetTimeOfDay(SaveGame->DayTime);
		DaySequenceActor->Play();
	}
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
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

FWorldState AWorldManager::GetWorldState() const
{
	FWorldState State;
	State.DayTime = DaySequenceActor->GetTimeOfDay();
	State.CurrentWeather = CurrentWeather;
	State.CurrentWeatherDuration = CurrentWeatherDuration;
	State.CurrentWeatherTargetDuration = CurrentWeatherTargetDuration;

	return State;
}

EWeatherType AWorldManager::GetNextWeather()
{
	TArray<EWeatherType> AllWeathers = { EWeatherType::Sunny, EWeatherType::Rain, EWeatherType::Cloudy, EWeatherType::Fog };
	AllWeathers.Remove(CurrentWeather);

	int32 Index = FMath::RandRange(0, AllWeathers.Num() - 1);
	return AllWeathers[Index];
}

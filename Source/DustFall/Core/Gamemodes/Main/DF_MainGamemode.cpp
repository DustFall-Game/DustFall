// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_MainGamemode.h"
#include "DustFall/Saves/DF_SaveGame.h"
#include "DustFall/Systems/SaveGameManager/SaveGameManager.h"
#include "DustFall/Systems/Structures/SaveGameInfo.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"


void ADF_MainGamemode::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle AutosaveTimerHandle;
	GetWorldTimerManager().SetTimer(
		AutosaveTimerHandle,
		this,
		&ADF_MainGamemode::HandleAutoSave,
		180.0f,
		true
	);
}

AActor* ADF_MainGamemode::ChoosePlayerStart_Implementation(AController* Player)
{
	FTransform SpawnTransform;
	
	if (auto SaveGame = USaveGameManager::GetSaveGame(GetWorld()))
	{
		SpawnTransform.SetLocation(SaveGame->PlayerLocation);
		
		return GetWorld()->SpawnActor<AActor>(APlayerStart::StaticClass(), SpawnTransform);
	}
	
	if (GetNumPlayers() == 0 || Player->IsLocalController())
	{
		TArray<AActor*> PlayerStarts;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), PlayerStarts);
		if (PlayerStarts.Num())
			return PlayerStarts[0];
	}
	
	if (auto FounderPawn = GetWorld()->GetFirstPlayerController()->GetPawn())
	{
		FVector Offset(FMath::RandRange(200.f, 400.f), FMath::RandRange(200.f, 400.f), 0.f);
		SpawnTransform.SetLocation(FounderPawn->GetActorLocation() + Offset);
		
		return GetWorld()->SpawnActor<AActor>(APlayerStart::StaticClass(), SpawnTransform);
	}
	
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ADF_MainGamemode::HandleAutoSave()
{
	UWorld* World = GetWorld();
	if (!World) return;

	auto PC = UGameplayStatics::GetPlayerController(World, 0);
	if (!PC || !PC->IsLocalController())
		return;

	FNewSaveGameInfo NewSaveGameInfo;
	NewSaveGameInfo.SaveSlotName = TEXT("Autosave");

	int32 LevelIndex = USaveGameManager::GetLevelIndex(World);
	if (LevelIndex < 0)
		return;

	NewSaveGameInfo.PlayerLevel = LevelIndex;
	
	if (auto Pawn = PC->GetPawn())
	{
		NewSaveGameInfo.PlayerLocation = Pawn->GetActorLocation();
		UE_LOG(LogTemp, Display, TEXT("Автосейв: %s на уровне %d"),
		*NewSaveGameInfo.PlayerLocation.ToString(), LevelIndex);
	}

	USaveGameManager::SaveGame(NewSaveGameInfo);
}

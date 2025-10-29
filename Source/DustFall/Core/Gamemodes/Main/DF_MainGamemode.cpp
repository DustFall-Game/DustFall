// Fill out your copyright notice in the Description page of Project Settings.


#include "DF_MainGamemode.h"
#include "DustFall/Saves/DF_SaveGame.h"
#include "DustFall/Systems/SaveGameManager/SaveGameManager.h"
#include "DustFall/Systems/Structures/SaveGameInfo.h"
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
	return nullptr;
}

void ADF_MainGamemode::HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer)
{
	Super::HandleStartingNewPlayer_Implementation(NewPlayer);
	if (!NewPlayer) return;
	
	FTransform SpawnTransform;
	FString SaveSlotName;
	const FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFiles(FoundFiles, *SaveDir, TEXT("*.sav"));

	const FString LevelPrefix = FString::Printf(TEXT("Autosave-%d"), USaveGameManager::GetLevelIndex(GetWorld()));
	for (const FString& FileName : FoundFiles)
	{
		if (FileName.StartsWith(LevelPrefix))
		{
			SaveSlotName = FPaths::GetBaseFilename(FileName);
			break;
		}
	}
	
	if (!SaveSlotName.IsEmpty())
	{
		if (auto LoadedGame = Cast<UDF_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
			SpawnTransform.SetLocation(LoadedGame->PlayerLocation);
	} else {
		if (GetNumPlayers() == 0)
		{
			if (AActor* PlayerStart = ChoosePlayerStart(NewPlayer))
				SpawnTransform = PlayerStart->GetActorTransform();
		} else {
			if (auto FounderPC = GetWorld()->GetFirstPlayerController())
				if (auto
					FounderPawn = FounderPC->GetPawn())
				{
					FVector FounderLocation = FounderPawn->GetActorLocation();
					FVector Offset(FMath::RandRange(200.f, 400.f), FMath::RandRange(200.f, 400.f), 0.f);
					SpawnTransform.SetLocation(FounderLocation + Offset);
				}
		}
	}
	
	if (auto SpawnedPawn = SpawnDefaultPawnAtTransform(NewPlayer, SpawnTransform))
		NewPlayer->Possess(SpawnedPawn);
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

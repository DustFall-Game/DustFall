// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveGameManager.h"
#include "IImageWrapper.h"
#include "IImageWrapperModule.h"
#include "DustFall/Saves/DF_SaveGame.h"
#include "Kismet/GameplayStatics.h"


UDF_SaveGame* USaveGameManager::GetSaveGame(UWorld* World)
{
	FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	TArray<FString> FoundFiles;
	IFileManager::Get().FindFiles(FoundFiles, *SaveDir, TEXT("*.sav"));
	
	FString LevelPrefix = FString::Printf(TEXT("Autosave-%d"), GetLevelIndex(World));
	FString* SaveFile = FoundFiles.FindByPredicate([&](const FString& FileName)
	{
		return FileName.StartsWith(LevelPrefix);
	});
	
	if (!SaveFile)
		return nullptr;

	return Cast<UDF_SaveGame>(UGameplayStatics::LoadGameFromSlot(FPaths::GetBaseFilename(*SaveFile), 0));
}

void USaveGameManager::SaveGame(const FNewSaveGameInfo& NewSaveGameInfo)
{
	FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames");
	IFileManager& FileManager = IFileManager::Get();
	
	FString LevelPrefix = FString::Printf(TEXT("Autosave-%d"), NewSaveGameInfo.PlayerLevel);
	FString ExistingSaveFile;
	
	TArray<FString> FoundFiles;
	FileManager.FindFiles(FoundFiles, *SaveDir, TEXT("*.sav"));
	for (const FString& FileName : FoundFiles)
	{
		if (FileName.StartsWith(LevelPrefix))
		{
			ExistingSaveFile = FileName;
			break;
		}
	}
	
	int64 TimeStamp = FDateTime::UtcNow().ToUnixTimestamp() * 1000 + FDateTime::UtcNow().GetMillisecond();
	FString UniqueSaveSlotName = LevelPrefix + FString::Printf(TEXT("%lld"), TimeStamp);

	auto SaveGameInstance = Cast<UDF_SaveGame>(UGameplayStatics::CreateSaveGameObject(UDF_SaveGame::StaticClass()));
	if (!SaveGameInstance) return;

	SaveGameInstance->SaveDate = FDateTime::Now();
	SaveGameInstance->SaveName = UniqueSaveSlotName;
	SaveGameInstance->PlayerLevel = NewSaveGameInfo.PlayerLevel;
	SaveGameInstance->PlayerLocation = NewSaveGameInfo.PlayerLocation;
	
	FString ScreenshotPath = SaveDir / (UniqueSaveSlotName + TEXT(".png"));
	SaveScreenshotToFile(ScreenshotPath);
	SaveGameInstance->ScreenshotPath = ScreenshotPath;
	
	UGameplayStatics::SaveGameToSlot(SaveGameInstance, UniqueSaveSlotName, 0);
	
	if (!ExistingSaveFile.IsEmpty())
	{
		FileManager.Delete(*(SaveDir / ExistingSaveFile));

		FString OldScreenshotPath = FPaths::ChangeExtension(SaveDir / ExistingSaveFile, TEXT(".png"));
		if (FPaths::FileExists(OldScreenshotPath))
			FileManager.Delete(*OldScreenshotPath);
	}
}

TArray<FSaveGameInfo> USaveGameManager::GetAllSaveGames()
{
	TArray<FSaveGameInfo> Saves;
	IFileManager& FileManager = IFileManager::Get();

	FString SaveDir = FPaths::ProjectSavedDir() / TEXT("SaveGames/");
	TArray<FString> FoundSaves;
	FileManager.FindFiles(FoundSaves, *SaveDir, TEXT("*.sav"));
	
	for (const FString& SaveFile : FoundSaves)
	{
		FString SaveName = FPaths::GetBaseFilename(SaveFile);
		if (auto Loaded = Cast<UDF_SaveGame>(UGameplayStatics::LoadGameFromSlot(SaveName, 0)))
		{
			FSaveGameInfo Info;
			Info.SaveName = Loaded->SaveName;
			Info.SaveDate = Loaded->SaveDate;

			if (FPaths::FileExists(Loaded->ScreenshotPath))
				Info.SaveScreenshot = LoadTextureFromFile(Loaded->ScreenshotPath);

			Saves.Add(Info);
		}
	}

	return Saves;
}

int32 USaveGameManager::GetLevelIndex(UWorld* World)
{
	if (!World) return -1;

	FString LevelName = World->GetMapName();
	LevelName.RemoveFromStart(TEXT("UEDPIE_0_"));
	LevelName.RemoveFromStart(TEXT("UEDPIE_1_"));

	static const TMap<FString, int32> LevelIndexes = {
		{ TEXT("TestMechanicsMap"), 1 },
	};

	if (const int32* Found = LevelIndexes.Find(LevelName))
		return *Found;

	UE_LOG(LogTemp, Warning, TEXT("Не найден индекс для карты: %s"), *LevelName);
	return -1;
}

void USaveGameManager::SaveScreenshotToFile(const FString& FilePath)
{
	FScreenshotRequest::RequestScreenshot(FilePath, false, false);
}

UTexture2D* USaveGameManager::LoadTextureFromFile(const FString& ImagePath)
{
	TArray<uint8> ImageData;
	if (!FFileHelper::LoadFileToArray(ImageData, *ImagePath)) return nullptr;

	IImageWrapperModule& ImageWrapperModule = FModuleManager::LoadModuleChecked<IImageWrapperModule>(FName("ImageWrapper"));
	TSharedPtr<IImageWrapper> ImageWrapper = ImageWrapperModule.CreateImageWrapper(EImageFormat::PNG);

	if (ImageWrapper->SetCompressed(ImageData.GetData(), ImageData.Num()))
	{
		TArray64<uint8> RawData;
		if (ImageWrapper->GetRaw(ERGBFormat::BGRA, 8, RawData))
		{
			UTexture2D* Texture = UTexture2D::CreateTransient(
				ImageWrapper->GetWidth(),
				ImageWrapper->GetHeight(),
				PF_B8G8R8A8
			);
			void* TextureData = Texture->GetPlatformData()->Mips[0].BulkData.Lock(LOCK_READ_WRITE);
			FMemory::Memcpy(TextureData, RawData.GetData(), RawData.Num());
			Texture->GetPlatformData()->Mips[0].BulkData.Unlock();
			Texture->UpdateResource();
			
			return Texture;
		} else {
			UE_LOG(LogTemp, Warning, TEXT("Failed to decode image: %s"), *ImagePath);
		}
	} else {
		UE_LOG(LogTemp, Warning, TEXT("Invalid image wrapper for: %s"), *ImagePath);
	}
	
	return nullptr;
}

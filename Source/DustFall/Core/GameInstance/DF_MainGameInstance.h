// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AdvancedFriendsGameInstance.h"
#include "BlueprintDataDefinitions.h"
#include "Engine/GameInstance.h"
#include "DF_MainGameInstance.generated.h"

class UFindSessionsCallbackProxyAdvanced;
class UCreateSessionCallbackProxyAdvanced;
class IOnlineSubsystem;
/**
 * 
 */
UCLASS()
class DUSTFALL_API UDF_MainGameInstance : public UAdvancedFriendsGameInstance
{
	GENERATED_BODY()
	
public:
	TArray<FOnlineSessionSearchResult> OnlineSessionResults;
	
	UFUNCTION(BlueprintCallable)
	void AdvancedCreateSession(const FString& SessionName);

	UFUNCTION(BlueprintCallable)
	UFindSessionsCallbackProxyAdvanced* AdvancedFindSessions(const FString& SessionName);

	UFUNCTION(BlueprintCallable)
	void AdvancedJoinSession(const FString& SessionName, const int32& SessionIndex);

	virtual void Init() override;
	virtual void InitUniquePlayerId();

	/** Sounds */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundMix* MainSoundMix;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* MasterVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* VoiceVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* SFXVolumeClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Audio")
	USoundClass* AmbientVolumeClass;

private:
	IOnlineSessionPtr SessionInterface;
	
	UPROPERTY()
	FUniqueNetIdRepl UniquePlayerNetId;
	
	UPROPERTY()
	UCreateSessionCallbackProxyAdvanced* CreateProxySession;

	/** Delegates */
	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteHandle;

	/** Internal callbacks */
	UFUNCTION()
	void OnCreateSessionSuccess();

	UFUNCTION()
	void OnCreateSessionFailure();

	virtual void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);
};

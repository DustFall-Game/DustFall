// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIHabitatZone.generated.h"

class UBoxComponent;
class AAIController;

USTRUCT(BlueprintType)
struct FSpawnEntry
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> PawnClass;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ClampMin="0", ClampMax="128"))
	int32 DesiredCount = 0;
};

UCLASS()
class DUSTFALL_API AAIHabitatZone : public AActor
{
	GENERATED_BODY()

public:
	AAIHabitatZone();
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Zone")
	UBoxComponent* ZoneBox;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zone")
	TArray<FSpawnEntry> SpawnList;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Zone", meta=(ClampMin="0"))
	float RespawnDelay = 30.0f; // 360

	UPROPERTY()
	TArray<TWeakObjectPtr<APawn>> TrackedMobs;

protected:
	virtual void BeginPlay() override;
	virtual void EnsureSpawnCounts();
	virtual void CleanupTrackedList();
	virtual void CommandReturnToZone(APawn* Pawn);
	virtual void RespawnCallback(TSubclassOf<APawn> PawnClass);
	virtual APawn* SpawnOneForEntry(const FSpawnEntry& Entry);
	virtual FVector GetSafeSpawnPoint();

	UFUNCTION()
	void OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void OnTrackedMobDestroyed(AActor* DestroyedActor);
	
	TMap<TSubclassOf<APawn>, FTimerHandle> RespawnTimers;
	TArray<FVector> UsedSpawnPoints;
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "AIHabitatZone.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/BoxComponent.h"
#include "DustFall/AI/Base/Interfaces/AIHabitatInterface.h"


AAIHabitatZone::AAIHabitatZone()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    ZoneBox = CreateDefaultSubobject<UBoxComponent>(TEXT("ZoneBox"));
    ZoneBox->SetCollisionProfileName(TEXT("Trigger"));
    RootComponent = ZoneBox;

    ZoneBox->OnComponentBeginOverlap.AddDynamic(this, &AAIHabitatZone::OnZoneBeginOverlap);
    ZoneBox->OnComponentEndOverlap.AddDynamic(this, &AAIHabitatZone::OnZoneEndOverlap);
}

void AAIHabitatZone::BeginPlay()
{
    Super::BeginPlay();
    
    EnsureSpawnCounts();
}

void AAIHabitatZone::EnsureSpawnCounts()
{
    if (!HasAuthority()) return;

    CleanupTrackedList();
    
    TMap<TSubclassOf<APawn>, int32> CurrentCounts;
    for (auto& W : TrackedMobs)
    {
        APawn* P = W.Get();
        if (P && P->GetClass())
        {
            CurrentCounts.FindOrAdd(P->GetClass())++;
        }
    }

    for (const FSpawnEntry& Entry : SpawnList)
    {
        int32 Have = CurrentCounts.FindRef(Entry.PawnClass);
        int32 Need = Entry.DesiredCount - Have;
        for (int i = 0; i < Need; ++i)
        {
            APawn* New = SpawnOneForEntry(Entry);
            if (New)
            {
                TrackedMobs.Add(New);

                New->OnDestroyed.AddDynamic(this, &AAIHabitatZone::OnTrackedMobDestroyed);
            }
        }
    }
}

APawn* AAIHabitatZone::SpawnOneForEntry(const FSpawnEntry& Entry)
{
    if (!Entry.PawnClass || !HasAuthority()) return nullptr;
    
    UWorld* World = GetWorld();
    if (!World) return nullptr;
    
    FVector Location = GetSafeSpawnPoint();
    FRotator Rotation = FRotator::ZeroRotator;
    
    FActorSpawnParameters SpawnParams;
    SpawnParams.Owner = this;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
    
    auto Pawn = World->SpawnActor<APawn>(Entry.PawnClass, Location, Rotation, SpawnParams);
    if (!Pawn) return nullptr;

    if (!Pawn->GetController())
        Pawn->SpawnDefaultController();
    
    IAIHabitatInterface::Execute_SetHabitatZone(Pawn, this);

    return Pawn;
}

FVector AAIHabitatZone::GetSafeSpawnPoint()
{
    UWorld* World = GetWorld();
    if (!World || !ZoneBox)
        return GetActorLocation();

    const FVector Origin = ZoneBox->GetComponentLocation();
    const FVector Extent = ZoneBox->GetScaledBoxExtent();
    
    auto NavSystem = UNavigationSystemV1::GetCurrent(World);
    for (int Attempt = 0; Attempt < 60; ++Attempt)
    {
        FVector Base(
            FMath::FRandRange(-Extent.X, Extent.X),
            FMath::FRandRange(-Extent.Y, Extent.Y),
            Origin.Z + Extent.Z
        );
        Base += Origin;
        
        FVector Start = Base + FVector(0.f, 0.f, 3000.f);
        FVector End = Base - FVector(0.f, 0.f, 3000.f);

        FHitResult Hit;
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(this);
        Params.bTraceComplex = true;

        if (!World->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
            continue;

        float RandomZ = FMath::FRandRange(50.f, 150.f);
        FVector Ground = Hit.ImpactPoint + FVector(0.f, 0.f, RandomZ);

        if (NavSystem)
        {
            bool bIsProjected = false;
            FNavLocation NavLocation;
            FVector ProjectExtent(100.f, 100.f, 300.f);
            bIsProjected = NavSystem->ProjectPointToNavigation(Ground, NavLocation, ProjectExtent);
            
            Ground = (bIsProjected ? NavLocation.Location : Hit.ImpactPoint) + FVector(0.f, 0.f, RandomZ);
        }
        
        bool bBlocked = World->OverlapBlockingTestByChannel(
            Ground, FQuat::Identity, ECC_Pawn,
            FCollisionShape::MakeCapsule(40.f, 88.f)
        );

        if (bBlocked) continue;
        
        bool bTooClose = false;
        for (const FVector& Used : UsedSpawnPoints)
        {
            float Distance2D = FVector2D::Distance(FVector2D(Used), FVector2D(Ground));

            if (Distance2D < 200.f)
            {
                bTooClose = true;
                break;
            }
        }
        if (bTooClose) continue;
        
        UsedSpawnPoints.Add(Ground);
        return Ground;
    }
    
    float FallbackZ = Origin.Z + 150.f;
    FVector Fallback = Origin + FVector(0.f, 0.f, FallbackZ - Origin.Z);
    
    return Fallback;
}

void AAIHabitatZone::OnZoneBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    auto Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;

    for (auto& TrackedMob : TrackedMobs)
    {
        if (TrackedMob.IsValid() && TrackedMob.Get() == Pawn)
        {
            FTimerHandle TimerHandle;
            GetWorld()->GetTimerManager().SetTimer(
                TimerHandle,
                [this, Pawn]
                {
                    if (auto AIController = Cast<AAIController>(Pawn->GetController()))
                        if (auto BB = AIController->GetBlackboardComponent())
                            BB->ClearValue("ZonePoint");
                },
                1.f,
                false);
        }
    }
}

void AAIHabitatZone::OnZoneEndOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    auto Pawn = Cast<APawn>(OtherActor);
    if (!Pawn) return;
    
    for (auto& TrackedMob : TrackedMobs)
    {
        if (TrackedMob.IsValid() && TrackedMob.Get() == Pawn)
        {
            CommandReturnToZone(Pawn);
            return;
        }
    }
}

void AAIHabitatZone::CommandReturnToZone(APawn* Pawn)
{
    if (!Pawn) return;

    if (auto AIController = Cast<AAIController>(Pawn->GetController()))
        if (auto BB = AIController->GetBlackboardComponent())
            BB->SetValueAsVector("ZonePoint", GetSafeSpawnPoint());
}

void AAIHabitatZone::OnTrackedMobDestroyed(AActor* DestroyedActor)
{
    auto Pawn = Cast<APawn>(DestroyedActor);
    if (!Pawn) return;
    
    for (int i = TrackedMobs.Num() - 1; i >= 0; --i)
    {
        if (!TrackedMobs[i].IsValid() || TrackedMobs[i].Get() == Pawn)
            TrackedMobs.RemoveAt(i);
    }
    
    TSubclassOf<APawn> K = Pawn->GetClass();
    bool bIsInSpawnList = false;
    for (const FSpawnEntry& E : SpawnList)
    {
        if (E.PawnClass == K && E.DesiredCount > 0)
        {
            bIsInSpawnList = true;
            break;
        }
    }

    if (bIsInSpawnList)
        if (!RespawnTimers.Contains(K) || !GetWorldTimerManager().IsTimerActive(RespawnTimers[K]))
        {
            FTimerHandle Handle;
            FTimerDelegate Delegate;
            Delegate.BindUObject(this, &AAIHabitatZone::RespawnCallback, K);
            GetWorldTimerManager().SetTimer(Handle, Delegate, RespawnDelay, false);
            RespawnTimers.Add(K, Handle);
        }
}

void AAIHabitatZone::RespawnCallback(TSubclassOf<APawn> PawnClass)
{
    RespawnTimers.Remove(PawnClass);
    
    FSpawnEntry Temp;
    Temp.PawnClass = PawnClass;
    Temp.DesiredCount = 1;
    if (APawn* New = SpawnOneForEntry(Temp))
    {
        TrackedMobs.Add(New);
        New->OnDestroyed.AddDynamic(this, &AAIHabitatZone::OnTrackedMobDestroyed);
    }
    
    EnsureSpawnCounts();
}

void AAIHabitatZone::CleanupTrackedList()
{
    for (int i = TrackedMobs.Num()-1; i >= 0; --i)
    {
        if (!TrackedMobs[i].IsValid())
        {
            TrackedMobs.RemoveAt(i);
        }
    }
}
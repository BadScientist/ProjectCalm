#include "ActorSpawnRegion.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "PhysicalMaterials/PhysicalMaterial.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

#ifdef PC_DEBUG_DRAW_SHAPES
	// #define LOCAL_DEBUG_DRAW_SHAPES
#endif


AActorSpawnRegion::AActorSpawnRegion()
{
    PrimaryActorTick.bCanEverTick = true;

#if WITH_EDITORONLY_DATA
    SpawnRegionVisComp = CreateDefaultSubobject<USpawnRegionVisualizerComponent>(TEXT("VisualizerComponent"));
    if (SpawnRegionVisComp != nullptr) {SetRootComponent(SpawnRegionVisComp);}
#endif
}

void AActorSpawnRegion::BeginPlay()
{
    Super::BeginPlay();

    PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogActor, "ActorSpawnRegion:: No Game Mode!");
    GameMode->OnLandscapeReady.AddDynamic(this, &AActorSpawnRegion::SetReadyToSpawn);
}

TArray<FSpawnCell> AActorSpawnRegion::CreateSpawnCells()
{
    TArray<FSpawnCell> Result;
    FVector RegionCenter = GetActorLocation();

    FVector CellSize = FVector(Size.X / HorizontalCells, Size.Y / VerticalCells, Size.Z);
    FVector FirstCellLocation = FVector(RegionCenter.X - Size.X/2 + CellSize.X/2, RegionCenter.Y - Size.Y/2 + CellSize.Y/2, RegionCenter.Z);

#ifdef LOCAL_DEBUG_DRAW_SHAPES
            GetWorld()->PersistentLineBatcher->SetComponentTickEnabled(false);
            DrawDebugBox(GetWorld(), FirstCellLocation, CellSize/2, FColor::Red, true, 100000.0f, ESceneDepthPriorityGroup::SDPG_MAX, 50.0f);
#endif

    for (int32 i = 0; i < VerticalCells; i++)
    {
        for (int32 j = 0; j < HorizontalCells; j++)
        {
            FVector CellLocation = FVector(FirstCellLocation.X + j * CellSize.X, FirstCellLocation.Y + i * CellSize.Y, FirstCellLocation.Z);

#ifdef LOCAL_DEBUG_DRAW_SHAPES
            DrawDebugBox(GetWorld(), CellLocation, CellSize/2,
            ((i + j) % 2 == 0) ? FColor::Blue : FColor::Cyan, true, 100000.0f,
            ((i + j) % 2 == 0) ? ESceneDepthPriorityGroup::SDPG_MAX : ESceneDepthPriorityGroup::SDPG_World,
            ((i + j) % 2 == 0) ? 10.0f : 25.0f);
#endif

            Result.Add(FSpawnCell(CellLocation, CellSize));
        }
    }
    
    return Result;
}

void AActorSpawnRegion::SpawnActors()
{
    TArray<FSpawnCell> SpawnCells;
    
    bool bIsComplete{true};
    for (FSpawnInfo SpawnInfo : SpawnList)
    {
        if (SpawnInfo.bHasSpawned) {continue;}

        if (SpawnCells.IsEmpty()) {SpawnCells = CreateSpawnCells();}

        int32 Spawns{0};
        int32 Attempts{0};
        while (Spawns < SpawnInfo.Count && Attempts < SpawnInfo.Count * 2)
        {
            int32 CellIdx = Attempts % SpawnCells.Num();
            if (SpawnActor(SpawnCells[CellIdx].GetRandomLocation(), SpawnInfo))
            {
                Spawns++;
            }
            Attempts++;
        }

        SpawnInfo.bHasSpawned = true;
        bIsComplete = bIsComplete & SpawnInfo.bHasSpawned;
    }

    bSpawningComplete = bIsComplete;

    if (bDestroyAfterSpawning) {Destroy();}
}

bool AActorSpawnRegion::SpawnActor(FVector Location, FSpawnInfo SpawnInfo)
{
    bool Result = false;
    FVector TraceStart = Location;
    FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - Size.Z);
    FHitResult OutHit;
    FCollisionShape Sphere = FCollisionShape::MakeSphere(SpawnInfo.SweepSphereRadius);
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = true;

    bool Hit = GetWorld()->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, Params);
    if (Hit)
    {
        if (SpawnInfo.ValidSurfaces.IsEmpty() || (OutHit.PhysMaterial.IsValid() && SpawnInfo.ValidSurfaces.Contains(OutHit.PhysMaterial->SurfaceType)))
        {
            AActor* SpawnedActor = GetWorld()->SpawnActor<AActor>(
                SpawnInfo.ActorClass.Get(),
                FVector(OutHit.ImpactPoint.X, OutHit.ImpactPoint.Y, OutHit.ImpactPoint.Z + SpawnInfo.SurfaceOffset),
                FRotator(0,0,0));

            if (SpawnedActor != nullptr)
            {
                SpawnedActors.Add(SpawnedActor);
                Result = true;
            }
        }
    }

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogTemp, Display, TEXT("%s:: Hit: %i, ValidPhysMat: %i, Result: %i"), *GetActorNameOrLabel(), Hit, OutHit.PhysMaterial.IsValid(), Result);
#endif // LOCAL_DEBUG_LOGS

    return Result;
}

void AActorSpawnRegion::SetActorsActive(bool bValue)
{
    for (AActor* SpawnedActor : SpawnedActors)
    {
        if (SpawnedActor == nullptr) {continue;}
        SpawnedActor->SetActorTickEnabled(bValue);
        SpawnedActor->SetActorHiddenInGame(!bValue);
    }
}

void AActorSpawnRegion::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (!bSpawningComplete && bReadyToSpawn) {SpawnActors();}

    CHECK_NULLPTR_RET(PlayerCharacter, LogActor, "ActorSpawnRegion:: No PlayerCharacter!");

    float DistanceToPlayer = FVector::Distance(PlayerCharacter->GetActorLocation(), GetActorLocation());
    if (DistanceToPlayer <= SpawnActivationDistance) {SetActorsActive(true);}
    else if (DistanceToPlayer >= UnrenderDistance) {SetActorsActive(false);}
}

#if WITH_EDITORONLY_DATA
IMPLEMENT_VISUALIZER(AActorSpawnRegion, Size);
#endif

#include "ActorSpawnRegion.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

// #if WITH_EDITOR
// #include "ProjectCalm/EditorOnly/SpawnRegionVisualizerComponent.h"
// #endif

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

// #if WITH_EDITOR
//     SpawnRegionVisComp = CreateDefaultSubobject<USpawnRegionVisualizerComponent>(TEXT("VisualizerComponent"));
//     if (SpawnRegionVisComp != nullptr) {SetRootComponent(SpawnRegionVisComp);}
// #endif
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
    UWorld* World = GetWorld();
    if (World != nullptr) {World->PersistentLineBatcher->SetComponentTickEnabled(false);}
    DrawDebugBox(World, FirstCellLocation, CellSize/2, FColor::Red, true, 100000.0f, ESceneDepthPriorityGroup::SDPG_MAX, 50.0f);
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

    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogActor, "ActorSpawnRegion:: Could not get World!", false);
    bool Hit = World->SweepSingleByChannel(OutHit, TraceStart, TraceEnd, FQuat::Identity, ECollisionChannel::ECC_Visibility, Sphere, Params);
    if (Hit)
    {
        if (SpawnInfo.ValidSurfaces.IsEmpty() || (OutHit.PhysMaterial.IsValid() && SpawnInfo.ValidSurfaces.Contains(OutHit.PhysMaterial->SurfaceType)))
        {
            AActor* SpawnedActor = World->SpawnActor<AActor>(
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

// #if WITH_EDITOR
// void AActorSpawnRegion::UpdateVisualizerComponentProperties() 
// {
//     SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
//     if (SpawnRegionVisComp == nullptr) {return;}
//     SpawnRegionVisComp->UpdateProperties(GetActorLocation(), GetActorRotation(), Size);
// }

// void AActorSpawnRegion::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
// {
//     Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
//     UpdateVisualizerComponentProperties();
// }

// void AActorSpawnRegion::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
// {
//     Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
//     UpdateVisualizerComponentProperties();
// }

// void AActorSpawnRegion::EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
// {
//     const FVector CurrentScale = GetRootComponent()->GetRelativeScale3D();
//     FVector SafeDeltaScale = FVector(FMath::Clamp(DeltaScale.X, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Y, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Z, -1.0f, 1.0f));
//     FVector ScalingVector = FVector(1.0f) + SafeDeltaScale;
//     Size *= ScalingVector;

//     if (PivotLocation)
//     {
//         const FRotator ActorRotation = GetActorRotation();
//         const FVector WorldDelta = GetActorLocation() - (*PivotLocation);
//         const FVector LocalDelta = (ActorRotation.GetInverse()).RotateVector(WorldDelta);
//         const FVector LocalScaledDelta = LocalDelta * (ScalingVector / FVector(1.0f));
//         const FVector WorldScaledDelta = ActorRotation.RotateVector(LocalScaledDelta);
//         SetActorLocation(WorldScaledDelta + (*PivotLocation));
//     }

//     UpdateVisualizerComponentProperties();
// }

// void AActorSpawnRegion::PostEditChangeChainProperty(FPropertyChangedChainEvent &EditEvent)
// {
//     Super::PostEditChangeChainProperty(EditEvent);
//     if (!EditEvent.PropertyChain.IsEmpty() && EditEvent.PropertyChain.GetHead() != nullptr)
//     {
//         if (FProperty* EditedProperty = EditEvent.PropertyChain.GetHead()->GetValue())
//         {
//             if (EditedProperty->GetNameCPP() == "RelativeLocation" || EditedProperty->GetNameCPP() == "RelativeRotation" || EditedProperty->GetNameCPP() == "Size")
//             {
//                 UpdateVisualizerComponentProperties();
//             }
//         }
//     }
// }

// void AActorSpawnRegion::PostEditUndo()
// {
//     Super::PostEditUndo();
//     UpdateVisualizerComponentProperties();
// }
// #endif

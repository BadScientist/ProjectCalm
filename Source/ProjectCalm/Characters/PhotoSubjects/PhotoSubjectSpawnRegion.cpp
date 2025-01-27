#include "PhotoSubjectSpawnRegion.h"
#include "PhotoSubjectComponent.h"


APhotoSubjectSpawnRegion::APhotoSubjectSpawnRegion()
{
#if WITH_EDITORONLY_DATA
    SpawnRegionVisComp = CreateDefaultSubobject<USpawnRegionVisualizerComponent>(TEXT("VisualizerComponent"));
    if (SpawnRegionVisComp != nullptr) {SetRootComponent(SpawnRegionVisComp);}
#endif
}

void APhotoSubjectSpawnRegion::Initialize()
{
    if (bInitialized) {return;}

    for (int32 i = 0; i < SpawnableSubjects.Num(); i++)
    {
        if (SpawnableSubjects[i].SubjectClass == nullptr)
        {
            SpawnableSubjects.RemoveAt(i);
            --i;
        }
        else {TotalWeight += SpawnableSubjects[i].Weight;}
    }

    bInitialized = true;
}

void APhotoSubjectSpawnRegion::BeginPlay()
{
    Super::BeginPlay();

    Initialize();
}

int32 APhotoSubjectSpawnRegion::PickSubject()
{
    float Selector = FMath::RandRange(0.0f, TotalWeight);
    float CurrentWeight {0};
    for (int32 i = 0; i < SpawnableSubjects.Num(); i++)
    {
        float NextWeight = CurrentWeight + SpawnableSubjects[i].Weight;
        if (CurrentWeight < Selector && Selector <= NextWeight)
        {
            if (SpawnableSubjects[i].ActiveSpawns < SpawnableSubjects[i].Max) {return i;}
            return -1;
        }
        
        CurrentWeight = NextWeight;
    }
    return -1;
}

bool APhotoSubjectSpawnRegion::Contains2D(FVector TestLocation)
{
    FVector RegionLocation = GetActorLocation();
    FVector TestOffset = TestLocation - RegionLocation;
    FVector AdjustedTestOffset = TestOffset.RotateAngleAxis(-1 * GetActorRotation().Yaw, FVector::UpVector);
    FVector AdjustedTestLocation = RegionLocation + AdjustedTestOffset;

    bool bIsInRegion = RegionLocation.X - (Size.X / 2) <= AdjustedTestLocation.X \
        && AdjustedTestLocation.X <= RegionLocation.X + (Size.X / 2) \
        && RegionLocation.Y - (Size.Y / 2) <= AdjustedTestLocation.Y \
        && AdjustedTestLocation.Y <= RegionLocation.Y + (Size.Y / 2);
        
    return bIsInRegion;
}

bool APhotoSubjectSpawnRegion::SpawnPhotoSubject(FVector SpawnLocation)
{
    int32 SubjectIdx = PickSubject();
    if (SubjectIdx < 0) {return false;}

    // Construct BP Actor at the upper plane of the SpawnRegion
    FVector SafeSpawnLocation = FVector(SpawnLocation.X, SpawnLocation.Y, GetActorLocation().Z);
    AActor* SpawnedSubject = GetWorld()->SpawnActor<AActor>(
        SpawnableSubjects[SubjectIdx].SubjectClass.Get(),
        SafeSpawnLocation,
        FRotator(0,0,0));
    if (SpawnedSubject == nullptr) {return false;}

    UPhotoSubjectComponent* SubjectComp = SpawnedSubject->FindComponentByClass<UPhotoSubjectComponent>();
    if (SubjectComp == nullptr) {return !SpawnedSubject->Destroy();}

    // Move Actor to the ground
    // On success, track spawn counts
    // On failure Destroy Actor
    if (SubjectComp->Spawn(Size.Z))
    {
        SpawnedSubjects.Add(SpawnedSubject);
        SpawnableSubjects[SubjectIdx].ActiveSpawns += 1;
        return true;
    }

    return !SpawnedSubject->Destroy();
}

int32 APhotoSubjectSpawnRegion::CleanupSpawns(AActor* Player)
{
    int32 DespawnedActorCount{0};
    if (Player == nullptr) {return DespawnedActorCount;}

    for (int32 i = 0; i < SpawnedSubjects.Num(); i++)
    {
        UPhotoSubjectComponent* SubjectComp = SpawnedSubjects[i]->FindComponentByClass<UPhotoSubjectComponent>();
        if (SubjectComp == nullptr) {continue;}

        if (SubjectComp->Despawn(Player)) 
        {
            for (int32 j = 0; j < SpawnableSubjects.Num(); j++)
            {
                if (SpawnedSubjects[i]->IsA(SpawnableSubjects[j].SubjectClass))
                {
                    SpawnableSubjects[j].ActiveSpawns -= 1;
                    break;
                }
            }
            
            SpawnedSubjects.RemoveAt(i);
            DespawnedActorCount++;
            i--;
        }
    }

    return DespawnedActorCount;
}

#if WITH_EDITORONLY_DATA
IMPLEMENT_VISUALIZER(APhotoSubjectSpawnRegion, Size);
#endif

#include "PhotoSubjectSpawnRegion.h"
#include "PhotoSubjectComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"


APhotoSubjectSpawnRegion::APhotoSubjectSpawnRegion()
{
#if WITH_EDITOR
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

    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogActor, "PhotoSubjectSpawnRegion:: Could not get World!", false);

    // Construct BP Actor at the upper plane of the SpawnRegion
    FVector SafeSpawnLocation = FVector(SpawnLocation.X, SpawnLocation.Y, GetActorLocation().Z);
    AActor* SpawnedSubject = World->SpawnActor<AActor>(
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

#if WITH_EDITOR
void APhotoSubjectSpawnRegion::UpdateVisualizerComponentProperties() 
{
    SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0));
    if (SpawnRegionVisComp == nullptr) {return;}
    SpawnRegionVisComp->UpdateProperties(GetActorLocation(), GetActorRotation(), Size);
}

void APhotoSubjectSpawnRegion::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
    Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown);
    UpdateVisualizerComponentProperties();
}

void APhotoSubjectSpawnRegion::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
    Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown);
    UpdateVisualizerComponentProperties();
}

void APhotoSubjectSpawnRegion::EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown)
{
    const FVector CurrentScale = GetRootComponent()->GetRelativeScale3D();
    FVector SafeDeltaScale = FVector(FMath::Clamp(DeltaScale.X, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Y, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Z, -1.0f, 1.0f));
    FVector ScalingVector = FVector(1.0f) + SafeDeltaScale;
    Size *= ScalingVector;

    if (PivotLocation)
    {
        const FRotator ActorRotation = GetActorRotation();
        const FVector WorldDelta = GetActorLocation() - (*PivotLocation);
        const FVector LocalDelta = (ActorRotation.GetInverse()).RotateVector(WorldDelta);
        const FVector LocalScaledDelta = LocalDelta * (ScalingVector / FVector(1.0f));
        const FVector WorldScaledDelta = ActorRotation.RotateVector(LocalScaledDelta);
        SetActorLocation(WorldScaledDelta + (*PivotLocation));
    }

    UpdateVisualizerComponentProperties();
}

void APhotoSubjectSpawnRegion::PostEditChangeChainProperty(FPropertyChangedChainEvent &EditEvent)
{
    Super::PostEditChangeChainProperty(EditEvent);
    if (!EditEvent.PropertyChain.IsEmpty() && EditEvent.PropertyChain.GetHead() != nullptr)
    {
        if (FProperty* EditedProperty = EditEvent.PropertyChain.GetHead()->GetValue())
        {
            if (EditedProperty->GetNameCPP() == "RelativeLocation" || EditedProperty->GetNameCPP() == "RelativeRotation" || EditedProperty->GetNameCPP() == "Size")
            {
                UpdateVisualizerComponentProperties();
            }
        }
    }
}

void APhotoSubjectSpawnRegion::PostEditUndo()
{
    Super::PostEditUndo();
    UpdateVisualizerComponentProperties();
}
#endif

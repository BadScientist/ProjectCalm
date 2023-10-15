#include "PhotoSubjectSpawnRegion.h"
#include "PhotoSubjectComponent.h"

#if WITH_EDITORONLY_DATA
#include "EditorOnly/SpawnRegionVisualizerComponent.h"
#endif


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
            UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: SubjectClass == Nullptr for SpawnableSubjects[%i]"), i);
            SpawnableSubjects.RemoveAt(i);
            --i;
        }
        else
        {
            UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: SubjectClass != Nullptr for SpawnableSubjects[%i]"), i);
            TotalWeight += SpawnableSubjects[i].Weight;
        }
    }

    bInitialized = true;
}

void APhotoSubjectSpawnRegion::BeginPlay()
{
    Initialize();
}

int32 APhotoSubjectSpawnRegion::PickSubject()
{
    float Selector = FMath::RandRange(0.0f, TotalWeight);
    float CurrentWeight {0};
    for (int32 i = 0; i < SpawnableSubjects.Num(); i++)
    {
        float NextWeight = CurrentWeight + SpawnableSubjects[i].Weight;
        UE_LOG(LogTemp, Display, TEXT("SpawnRegion:: W: %f | S: %f | T: %f"), CurrentWeight, Selector, TotalWeight);
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
    if (RegionLocation.X - Size.X <= TestLocation.X \
        && TestLocation.X <= RegionLocation.X + Size.X \
        && RegionLocation.Y - Size.Y <= TestLocation.Y \
        && TestLocation.Y <= RegionLocation.Y + Size.Y) {return true;}

    return false;
}

bool APhotoSubjectSpawnRegion::SpawnPhotoSubject(FVector SpawnLocation)
{
    int32 SubjectIdx = PickSubject();
    if (SubjectIdx < 0) 
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: Invalid Index"));
        return false;
    }

    // Construct BP Actor at the upper plane of the SpawnRegion
    FVector SafeSpawnLocation = FVector(SpawnLocation.X, SpawnLocation.Y, GetActorLocation().Z);
    AActor* SpawnedSubject = GetWorld()->SpawnActor<AActor>(
        SpawnableSubjects[SubjectIdx].SubjectClass.Get(),
        SafeSpawnLocation,
        FRotator(0,0,0));
    if (SpawnedSubject == nullptr)
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: Failed to spawn Subject"));
        return false;
    }

    UPhotoSubjectComponent* SubjectComp = SpawnedSubject->FindComponentByClass<UPhotoSubjectComponent>();
    if (SubjectComp == nullptr) 
    {
        UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: Spawned Actor has no PhotoSubjectComponent"));
        return !SpawnedSubject->Destroy();
    }

    // Move Actor to the ground
    // On success, track spawn counts
    // On failure Destroy Actor
    if (SubjectComp->Spawn())
    {
        SpawnedSubjects.Add(SpawnedSubject);
        SpawnableSubjects[SubjectIdx].ActiveSpawns += 1;
        return true;
    }

    return !SpawnedSubject->Destroy();
}

int32 APhotoSubjectSpawnRegion::CleanupSpawns(FVector PlayerLocation, FVector PlayerForwardVector)
{
    int32 DespawnedActorCount{0};

    for (int32 i = 0; i < SpawnedSubjects.Num(); i++)
    {
        UPhotoSubjectComponent* SubjectComp = SpawnedSubjects[i]->FindComponentByClass<UPhotoSubjectComponent>();
        if (SubjectComp == nullptr) {continue;}

        UE_LOG(LogTemp, Warning, TEXT("SpawnRegion:: Attempting to despawn subject %i"), i);
        if (SubjectComp->Despawn(PlayerLocation, PlayerForwardVector)) 
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
// EDITOR-ONLY VISUALIZATION

void APhotoSubjectSpawnRegion::UpdateVisualizerComponentProperties()
{
    // Only allow rotation around Z-axis
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

    FVector SafeDeltaScale = FVector(
        FMath::Clamp(DeltaScale.X, -1.0f, 1.0f),
        FMath::Clamp(DeltaScale.Y, -1.0f, 1.0f),
        FMath::Clamp(DeltaScale.Z, -1.0f, 1.0f));
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

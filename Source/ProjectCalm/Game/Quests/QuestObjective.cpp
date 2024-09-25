#include "QuestObjective.h"
#include "ProjectCalm/Game/Quests/QuestDetails.h"
#include "ProjectCalm/Gameplay/InteractableActor.h"
#include "ProjectCalm/Gameplay/Proprietor.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"

#include "Components/SphereComponent.h"


AQuestObjective::AQuestObjective()
{
	PrimaryActorTick.bCanEverTick = false;
    ObjectiveType = EObjectiveType::OBJECTIVE_BASE;
}

void AQuestObjective::Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx)
{
    ObjectiveType = ObjectiveDetails.Type;
    QuestID = InQuestID;
    StageIdx = InStageIdx;
    ObjectiveIdx = InObjectiveIdx;
    bIsOptional = ObjectiveDetails.bIsOptional;
    bIsComplete = ObjectiveDetails.bIsComplete;
}

void AQuestObjective::Teardown()
{
    Destroy();
}

void AQuestObjective::CompleteObjective()
{
    bIsComplete = true;

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");

    GameMode->CompleteQuestObjective(this);
}

ATravelObjective::ATravelObjective() : AQuestObjective()
{
    ObjectiveType = EObjectiveType::TRAVEL;

    SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
    if (SphereCollision != nullptr)
    {
        SetRootComponent(SphereCollision);
        SphereCollision->SetCollisionProfileName("OverlapAll");
        SphereCollision->SetEnableGravity(false);
    }
}

void ATravelObjective::Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx)
{
    Super::Setup(ObjectiveDetails, InQuestID, InStageIdx, InObjectiveIdx);

    if (ObjectiveDetails.Type != EObjectiveType::TRAVEL) {Destroy(); return;}

    if (SphereCollision != nullptr) {SphereCollision->SetSphereRadius(ObjectiveDetails.Tolerance);}

    SetActorLocation(ObjectiveDetails.Destination);
}

void ATravelObjective::OnTriggerVolumeEntered(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (Cast<APlayerCharacter>(OtherActor) == nullptr) {return;}
    CompleteObjective();
}

void ATravelObjective::BeginPlay()
{
    Super::BeginPlay();

    if (SphereCollision != nullptr) {SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &ATravelObjective::OnTriggerVolumeEntered);}
}

AInteractObjective::AInteractObjective() : AQuestObjective()
{
    ObjectiveType = EObjectiveType::INTERACT;
}

void AInteractObjective::Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx)
{
    Super::Setup(ObjectiveDetails, InQuestID, InStageIdx, InObjectiveIdx);

    if (ObjectiveDetails.Type != ObjectiveType) {return;}

    InteractTargetClass = ObjectiveDetails.InteractTargetClass;

    if (ObjectiveDetails.bSpawnNewInteractable)
    {
        InteractTarget = GetWorld()->SpawnActor<AInteractableActor>(
            ObjectiveDetails.InteractTargetClass.Get(),
            ObjectiveDetails.TargetLocation,
            ObjectiveDetails.TargetRotation);
    }
}

void AInteractObjective::OnInteract(AInteractableActor* Interactable)
{
    if (InteractTarget != nullptr) 
    {
        if(Interactable != InteractTarget)
        {
            UE_LOG(LogTemp, Warning, TEXT("QuestObjective::OnInteract: Interactable is not InteractTarget"));
            return;
        }
    }
    else if (Interactable->GetClass() != InteractTargetClass.Get())
    {
        UE_LOG(LogTemp, Warning, TEXT("QuestObjective::OnInteract: Interactable class is not InteractTargetClass"));
        return;
    }
    CompleteObjective();
}

void AInteractObjective::BeginPlay()
{
    Super::BeginPlay();

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");

    GameMode->OnInteract.AddDynamic(this, &AInteractObjective::OnInteract);
}

APhotoObjective::APhotoObjective()
{
    ObjectiveType = EObjectiveType::PHOTO;
}

void APhotoObjective::Setup(FObjectiveDetails ObjectiveDetails, uint32 InQuestID, uint32 InStageIdx, uint32 InObjectiveIdx)
{
    Super::Setup(ObjectiveDetails, InQuestID, InStageIdx, InObjectiveIdx);

    if (ObjectiveDetails.Type != ObjectiveType) {return;}

    InteractTargetClass = AProprietor::StaticClass();

    PhotoTargets.Append(ObjectiveDetails.PhotoTargets);
    Score = ObjectiveDetails.Score;
}

void APhotoObjective::OnPhotoTaken(FPhotoData Photo)
{
    if (IsQualifiedPhoto(Photo)) {bHasQualifiedPhoto = true;}
}

void APhotoObjective::OnPhotoDeleted()
{
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");

    TArray<FPhotoData> AllPhotos;
    GameMode->GetAllPhotos(AllPhotos);
    for (FPhotoData Photo : AllPhotos) {if (IsQualifiedPhoto(Photo)) {return;}}

    bHasQualifiedPhoto = false;
}

void APhotoObjective::OnInteract(AInteractableActor* Interactable)
{
    if (bHasQualifiedPhoto) {Super::OnInteract(Interactable);}
}

void APhotoObjective::BeginPlay()
{
    Super::BeginPlay();

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");

    GameMode->OnPhotoTaken.AddDynamic(this, &APhotoObjective::OnPhotoTaken);
    GameMode->OnPhotoDeleted.AddDynamic(this, &APhotoObjective::OnPhotoDeleted);
}

bool APhotoObjective::IsQualifiedPhoto(FPhotoData Photo)
{    
    uint32 Matches{0};
    for (FSubjectBehaviorPair Target : PhotoTargets)
    {
        for (FPhotoSubjectData Subject : Photo.Subjects)
        {
            if (Subject.Name == Target.Subject && Subject.Behavior == Target.Behavior)
            {
                Matches++;
                break;
            }
        }
    }

    float ScoreTotal{0};
    for (FPhotoSubjectData Subject : Photo.Subjects)
    {
        for (FPhotoSubjectPointOfInterest POI : Subject.PointsOfInterest) {ScoreTotal += POI.ScoreValue;}
    }

    return Matches == PhotoTargets.Num() && ScoreTotal >= Score;
}

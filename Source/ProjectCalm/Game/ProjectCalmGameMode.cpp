// Copyright 2025 Joseph D Tong aka "BadScientist"

#include "ProjectCalmGameMode.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "PhotoManager.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "UObject/ConstructorHelpers.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

#ifdef PC_DEBUG_DRAW_SHAPES
	// #define LOCAL_DEBUG_DRAW_SHAPES
#endif


AProjectCalmGameMode::AProjectCalmGameMode() : Super()
{
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	ItemManager = CreateDefaultSubobject<UItemManager>(TEXT("ItemManager"));
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("QuestManager"));
	PhotoManager = CreateDefaultSubobject<UPhotoManager>(TEXT("PhotoManager"));
}

void AProjectCalmGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	CheckIsLandscapeReady();
}

void AProjectCalmGameMode::CheckIsLandscapeReady()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogGameMode, Display, TEXT("Checking Landscape..."));
#endif // DEBUG

    FVector TraceStart = FVector(-50000.0f, -50000.0f, 5.0f);
    FVector TraceEnd = FVector(-50000.0f, -50000.0f, -5.0f);
    FHitResult OutHit;
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = true;

    UWorld* World = GetWorld();
    CHECK_NULLPTR_RET(World, LogGameMode, "ProjectCalmGameMode:: Could not get World!");
    bool Hit = World->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, Params);
    if (Hit && OutHit.PhysMaterial.IsValid())
	{
#ifdef LOCAL_DEBUG_LOGS
		UE_LOG(LogGameMode, Display, TEXT("Landscape Ready!"));
#endif // DEBUG
		OnLandscapeReady.Broadcast();
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

int32 AProjectCalmGameMode::GenerateInstanceID()
{
	int32 Value = NextInstanceID;
	NextInstanceID++;
    return Value;
}

UItemData* AProjectCalmGameMode::GetItemDataFromID(int32 ItemID)
{
	return ItemManager->GetItemDataFromID(ItemID);
}

void AProjectCalmGameMode::SetSpecialRockLocation(FVector InLocation)
{
	SpecialRockLocation = InLocation;
	OnSpecialRockMoved.Broadcast(SpecialRockLocation);
}

FQuestDetails AProjectCalmGameMode::GetQuestDetails(uint32 QuestID)
{
	CHECK_NULLPTR_RETVAL(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!", FQuestDetails());
	return QuestManager->GetQuestDetails(QuestID);
}

void AProjectCalmGameMode::MarkIntroDialoguePlayed(uint32 QuestID, int32 StageID)
{
	CHECK_NULLPTR_RET(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!");
	QuestManager->MarkIntroDialoguePlayed(QuestID, StageID);
}

void AProjectCalmGameMode::GetActiveQuests(TArray<FQuestDetails> &OutArray, bool bIncludeHidden)
{
	if (QuestManager != nullptr) {QuestManager->GetActiveQuests(OutArray, bIncludeHidden);}
}

bool AProjectCalmGameMode::StartQuest(uint32 QuestID)
{
	CHECK_NULLPTR_RETVAL(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!", false);
    return QuestManager->StartQuest(QuestID);
}

bool AProjectCalmGameMode::SetQuestStage(uint32 QuestID, int32 StageID)
{
	CHECK_NULLPTR_RETVAL(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!", false);
    return QuestManager->SetStage(QuestID, StageID);
}

void AProjectCalmGameMode::CompleteQuestObjective(AQuestObjective *Objective)
{
	CHECK_NULLPTR_RET(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!");
	QuestManager->OnObjectiveCompleted(Objective);
}

APickup* AProjectCalmGameMode::SpawnPickup(int32 ItemID, FVector SpawnLocation, FRotator SpawnRotation)
{
	CHECK_NULLPTR_RETVAL(ItemManager, LogGameMode, "ProjectCalmGameMode:: No ItemManager found!", nullptr);
    return ItemManager->SpawnPickup(ItemID, SpawnLocation, SpawnRotation);
}

APickup* AProjectCalmGameMode::SpawnPickup(UItemData* ItemData, FVector SpawnLocation, FRotator SpawnRotation)
{
	CHECK_NULLPTR_RETVAL(ItemManager, LogGameMode, "ProjectCalmGameMode:: No ItemManager found!", nullptr);
    return ItemManager->SpawnPickup(ItemData, SpawnLocation, SpawnRotation);
}

void AProjectCalmGameMode::GetPhotos(int32 CameraID, TArray<FPhotoData>& OutPhotos)
{
	CHECK_NULLPTR_RET(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!");
	PhotoManager->GetPhotos(CameraID, OutPhotos);
}

void AProjectCalmGameMode::GetAllPhotos(TArray<FPhotoData>& OutPhotos)
{
	CHECK_NULLPTR_RET(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!");
	PhotoManager->GetAllPhotos(OutPhotos);
}

FPhotoData AProjectCalmGameMode::GetPhoto(int32 CameraID, uint32 PhotoIdx)
{
	CHECK_NULLPTR_RETVAL(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!", FPhotoData());
    return PhotoManager->GetPhoto(CameraID, PhotoIdx);
}

uint32 AProjectCalmGameMode::GetNumPhotos(int32 CameraID)
{
	CHECK_NULLPTR_RETVAL(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!", 0);
	return PhotoManager->GetNumPhotos(CameraID);
}

void AProjectCalmGameMode::AddPhoto(int32 CameraID, FPhotoData Photo)
{
	CHECK_NULLPTR_RET(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!");
	PhotoManager->AddPhoto(CameraID, Photo);
	OnPhotoTaken.Broadcast(Photo);
}

bool AProjectCalmGameMode::RemovePhoto(int32 CameraID, uint32 PhotoIdx)
{
	CHECK_NULLPTR_RETVAL(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!", false);
	bool bResult = PhotoManager->RemovePhoto(CameraID, PhotoIdx);
	if (bResult) {OnPhotoDeleted.Broadcast();}
    return bResult;
}

void AProjectCalmGameMode::ClearAllPhotos(int32 CameraID)
{
	CHECK_NULLPTR_RET(PhotoManager, LogGameMode, "ProjectCalmGameMode:: No PhotoManager found!");
	return PhotoManager->ClearAllPhotos(CameraID);
}

void AProjectCalmGameMode::PauseGame()
{
	OnGamePaused.Broadcast(0.00001f);
}

void AProjectCalmGameMode::UnpauseGame()
{
	OnGameUnpaused.Broadcast(1.0f);
}

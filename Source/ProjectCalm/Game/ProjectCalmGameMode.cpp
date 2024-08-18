// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCalmGameMode.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "PhotoManager.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "UObject/ConstructorHelpers.h"

AProjectCalmGameMode::AProjectCalmGameMode() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	ItemManager = CreateDefaultSubobject<UItemManager>(TEXT("ItemManager"));
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("QuestManager"));
	PhotoManager = CreateDefaultSubobject<UPhotoManager>(TEXT("PhotoManager"));
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

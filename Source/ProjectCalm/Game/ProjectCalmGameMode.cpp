// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCalmGameMode.h"
#include "ItemManager.h"
#include "QuestManager.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "UObject/ConstructorHelpers.h"

AProjectCalmGameMode::AProjectCalmGameMode() : Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	ItemManager = CreateDefaultSubobject<UItemManager>(TEXT("ItemManager"));
	QuestManager = CreateDefaultSubobject<UQuestManager>(TEXT("QuestManager"));
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

FQuestDetails AProjectCalmGameMode::GetQuestDetails(uint32 QuestID)
{
	CHECK_NULLPTR_RETVAL(QuestManager, LogGameMode, "ProjectCalmGameMode:: No QuestManager found!", FQuestDetails());
	return QuestManager->GetQuestDetails(QuestID);
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

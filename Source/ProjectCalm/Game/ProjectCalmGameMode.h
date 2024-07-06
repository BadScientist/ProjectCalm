// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

#include "ProjectCalm/Game/Quests/QuestDetails.h"
#include "ProjectCalm/Gameplay/InteractableActor.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalmGameMode.generated.h"

class UItemManager;
class UQuestManager;
class UItemData;
class AQuestObjective;
class APickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMakeNoiseDelegate, AActor*, ResponsibleActor, float, NoiseLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhotoTakenDelegate, FPhotoData, Photo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AInteractableActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdatedDelegate, FQuestDetails, QuestDetails);


UCLASS(minimalapi)
class AProjectCalmGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UItemManager* ItemManager;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UQuestManager* QuestManager;

	int32 NextInstanceID{0x0001};

public:
	AProjectCalmGameMode();

	FOnMakeNoiseDelegate OnMakeNoise;
	FOnPhotoTakenDelegate OnPhotoTaken;
	FOnInteract OnInteract;
	FOnQuestUpdatedDelegate OnQuestUpdated;

	int32 GenerateInstanceID();
	UItemData* GetItemDataFromID(int32 ItemID);

	FQuestDetails GetQuestDetails(uint32 QuestID);
	void GetActiveQuests(TArray<FQuestDetails>& OutArray, bool bIncludeHidden=false);
	bool StartQuest(uint32 QuestID);
    bool SetQuestStage(uint32 QuestID, int32 StageID);
	void CompleteQuestObjective(AQuestObjective* Objective);
	
	APickup* SpawnPickup(int32 ItemID, FVector SpawnLocation, FRotator SpawnRotation);
	APickup* SpawnPickup(UItemData* ItemData, FVector SpawnLocation, FRotator SpawnRotation);
};




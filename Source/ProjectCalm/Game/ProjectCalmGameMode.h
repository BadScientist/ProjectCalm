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
class UPhotoManager;
class UItemData;
class AQuestObjective;
class APickup;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMakeNoiseDelegate, AActor*, ResponsibleActor, float, NoiseLevel);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPhotoTakenDelegate, FPhotoData, Photo);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPhotoDeletedDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteract, AInteractableActor*, Interactable);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnQuestUpdatedDelegate, FQuestDetails, QuestDetails);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSpecialRockMoved, FVector, Location);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnPlayerRespawnDelegate);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnLandscapeReadyDelegate);


UCLASS(minimalapi)
class AProjectCalmGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UItemManager* ItemManager;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UQuestManager* QuestManager;

	UPROPERTY(EditDefaultsOnly, Category = Components)
	UPhotoManager* PhotoManager;

	int32 NextInstanceID{0x0001};

	FVector SpecialRockLocation{FVector::ZeroVector};

public:
	AProjectCalmGameMode();

	virtual void Tick(float DeltaTime) override;

private:
	bool bIsLandscapeReady{false};
	void CheckIsLandscapeReady();

public:
	FOnMakeNoiseDelegate OnMakeNoise;
	FOnPhotoTakenDelegate OnPhotoTaken;
	FOnPhotoDeletedDelegate OnPhotoDeleted;
	FOnInteract OnInteract;
	FOnQuestUpdatedDelegate OnQuestUpdated;
	FOnSpecialRockMoved OnSpecialRockMoved;
	FOnPlayerRespawnDelegate OnPlayerRespawn;

	UPROPERTY(BlueprintAssignable)
	FOnLandscapeReadyDelegate OnLandscapeReady;

	int32 GenerateInstanceID();
	UItemData* GetItemDataFromID(int32 ItemID);

	FVector GetSpecialRockLocation() {return SpecialRockLocation;};
	void SetSpecialRockLocation(FVector InLocation);

	FQuestDetails GetQuestDetails(uint32 QuestID);
	void MarkIntroDialoguePlayed(uint32 QuestID, int32 StageID);
	void GetActiveQuests(TArray<FQuestDetails>& OutArray, bool bIncludeHidden=false);
	bool StartQuest(uint32 QuestID);
    bool SetQuestStage(uint32 QuestID, int32 StageID);
	void CompleteQuestObjective(AQuestObjective* Objective);
	
	APickup* SpawnPickup(int32 ItemID, FVector SpawnLocation, FRotator SpawnRotation);
	APickup* SpawnPickup(UItemData* ItemData, FVector SpawnLocation, FRotator SpawnRotation);

	void GetPhotos(int32 CameraID, TArray<FPhotoData>& OutPhotos);
	void GetAllPhotos(TArray<FPhotoData>& OutPhotos);
	FPhotoData GetPhoto(int32 CameraID, uint32 PhotoIdx);
	uint32 GetNumPhotos(int32 CameraID);
	void AddPhoto(int32 CameraID, FPhotoData Photo);
	bool RemovePhoto(int32 CameraID, uint32 PhotoIdx);
	void ClearAllPhotos(int32 CameraID);
};




// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "ProjectCalm/Game/Quests/QuestDetails.h"
#include "QuestManager.generated.h"

class UDataTable;
class AQuestObjective;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UQuestManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
    UDataTable* QuestData;

	TMap<uint32, FQuestDetails> Quests;
	TMap<uint32, TArray<AQuestObjective*>> ActiveObjectives;

public:	
	// Sets default values for this component's properties
	UQuestManager();
	
	virtual void BeginPlay() override;

	virtual void OnObjectiveCompleted(AQuestObjective* Objective);

    bool StartQuest(uint32 QuestID);
    bool SetStage(uint32 QuestID, int32 StageIdx);

	FQuestDetails GetQuestDetails(uint32 QuestID);
	void GetActiveQuests(TArray<FQuestDetails>& OutArray, bool bIncludeHidden=false);

	void MarkIntroDialoguePlayed(uint32 QuestID, int32 StageID);

private:
	void CleanupActiveObjectives(uint32 QuestID);
	bool SpawnStageObjectives(uint32 QuestID, int32 StageIdx);
	AQuestObjective* SpawnObjective(FObjectiveDetails ObjectiveDetails);
	bool CheckStageComplete(FStageDetails Stage);
	
};

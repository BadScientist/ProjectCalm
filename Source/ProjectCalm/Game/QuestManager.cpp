#include "QuestManager.h"
#include "ProjectCalm/Game/Quests/QuestObjective.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "Engine/DataTable.h"

UQuestManager::UQuestManager()
{
}

void UQuestManager::BeginPlay()
{
    Super::BeginPlay();

    if (QuestData == nullptr) {return;}

    TArray<FQuestDetails*> Rows;
    QuestData->GetAllRows<FQuestDetails>(FString(), Rows);
    for (FQuestDetails* Row : Rows)
    {
        FQuestDetails Quest = *Row;
        Quests.Add(Quest.QuestID, Quest);

        if (Quest.IsActive()) {SpawnStageObjectives(Quest.QuestID, Quest.ActiveStageIdx);}
    }
}

void UQuestManager::OnObjectiveCompleted(AQuestObjective* Objective)
{
    uint32 QuestID = Objective->GetQuestID();
    uint32 StageIdx = Objective->GetStageIdx();
    uint32 ObjectiveIdx = Objective->GetObjectiveIdx();

    if (Quests.Contains(QuestID))
    {
        if (StageIdx < (uint32)Quests[QuestID].Stages.Num())
        {
            if (ObjectiveIdx < (uint32)Quests[QuestID].Stages[StageIdx].Objectives.Num())
            {
                Quests[QuestID].Stages[StageIdx].Objectives[ObjectiveIdx].bIsComplete = true;
            }
            if (CheckStageComplete(Quests[QuestID].Stages[StageIdx]))
            {
                Quests[QuestID].Stages[StageIdx].bIsComplete = true;                
                SetStage(QuestID, Quests[QuestID].ActiveStageIdx + 1);
            }
        }

        AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
        CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestManager:: No ProjectCalmGameMode found!");        
        GameMode->OnQuestUpdated.Broadcast(Quests[QuestID]);

        APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
        CHECK_NULLPTR(PlayerCharacter, LogQuest, "QuestManager:: No PlayerCharacter found!");
        if (Quests[QuestID].IsComplete() && PlayerCharacter != nullptr)
        {
            PlayerCharacter->NotifyPlayer(FString::Printf(TEXT("Quest Complete: %s"), *Quests[QuestID].Name.ToString()));
        }
    }
}

bool UQuestManager::StartQuest(uint32 QuestID)
{
    if (SetStage(QuestID, 0)) 
    {
        PCPlayerStatics::SendOnScreenNotification(this, "Quest Started: " + Quests[QuestID].Name.ToString());
        return true;
    }
    return false;
}

bool UQuestManager::SetStage(uint32 QuestID, int32 StageIdx)
{
    if (Quests.Contains(QuestID))
    {

        if (StageIdx >= 0 && StageIdx <= Quests[QuestID].Stages.Num())
        {
            Quests[QuestID].ActiveStageIdx = StageIdx;

            CleanupActiveObjectives(QuestID);

            if (Quests[QuestID].IsComplete()) {return true;}

            return SpawnStageObjectives(QuestID, StageIdx);
        }
    }
    return false;
}

FQuestDetails UQuestManager::GetQuestDetails(uint32 QuestID)
{
    if (Quests.Contains(QuestID)) {return Quests[QuestID];}
    return FQuestDetails();
}

void UQuestManager::GetActiveQuests(TArray<FQuestDetails>& OutArray, bool bIncludeHidden)
{
    TArray<uint32> QuestIDs;
    ActiveObjectives.GetKeys(QuestIDs);
    for (uint32 QuestID : QuestIDs)
    {
        if (Quests.Contains(QuestID) && (!Quests[QuestID].bIsHidden || bIncludeHidden))
        {
            OutArray.AddUnique(Quests[QuestID]);
        }
    }
}

void UQuestManager::MarkIntroDialoguePlayed(uint32 QuestID, int32 StageID)
{
    if (Quests.Contains(QuestID))
    {
        if (StageID >= 0 && StageID < Quests[QuestID].Stages.Num())
        {
            Quests[QuestID].Stages[StageID].bIntroPlayed = true;
        }
    }
}

void UQuestManager::CleanupActiveObjectives(uint32 QuestID)
{
    if (ActiveObjectives.Contains(QuestID))
    {
        TArray<AQuestObjective*> OldObjectives;
        for (AQuestObjective* Objective : ActiveObjectives[QuestID])
        {
            if (Objective != nullptr)
            {
                if (Objective->GetQuestID() == QuestID) {Objective->Teardown();}
                else {continue;}
            }
            OldObjectives.Add(Objective);
        }

        for (AQuestObjective* Objective : OldObjectives) {ActiveObjectives[QuestID].Remove(Objective);}
    }

    TArray<uint32> OldQuests;
    for (TPair<uint32, TArray<AQuestObjective*>> Quest : ActiveObjectives) {if (Quest.Value.IsEmpty()) {OldQuests.Add(Quest.Key);}}
    for (uint32 OldQuest : OldQuests) {ActiveObjectives.Remove(OldQuest);}
}

bool UQuestManager::SpawnStageObjectives(uint32 QuestID, int32 StageIdx)
{
    if (!Quests.Contains(QuestID) || StageIdx < 0 || StageIdx >= Quests[QuestID].Stages.Num()) {return false;}

    bool bResult{true};
    uint32 ObjectiveIdx{0};
    for (FObjectiveDetails ObjectiveDetails : Quests[QuestID].Stages[StageIdx].Objectives)
    {
        AQuestObjective* NewObjective = SpawnObjective(ObjectiveDetails);
        if (NewObjective == nullptr)
        {
            bResult = false;
            continue;
        }

        NewObjective->Setup(ObjectiveDetails, QuestID, StageIdx, ObjectiveIdx);

        if (!ActiveObjectives.Contains(QuestID)) {ActiveObjectives.Add(QuestID);}
        ActiveObjectives[QuestID].Add(NewObjective);
        ObjectiveIdx++;
    }

    return bResult;
}

AQuestObjective *UQuestManager::SpawnObjective(FObjectiveDetails ObjectiveDetails)
{
    switch (ObjectiveDetails.Type)
    {
    case EObjectiveType::TRAVEL:        
        return GetWorld()->SpawnActor<ATravelObjective>(FVector(), FRotator());
    case EObjectiveType::INTERACT:
        return GetWorld()->SpawnActor<AInteractObjective>(FVector(), FRotator());
    case EObjectiveType::PHOTO:        
        return GetWorld()->SpawnActor<APhotoObjective>(FVector(), FRotator());
    
    default:
        return GetWorld()->SpawnActor<AQuestObjective>(FVector(), FRotator());
    }
    return nullptr;
}

bool UQuestManager::CheckStageComplete(FStageDetails Stage)
{
    for (FObjectiveDetails Objective : Stage.Objectives) {if (!Objective.bIsComplete && !Objective.bIsOptional) {return false;}}
    return true;
}

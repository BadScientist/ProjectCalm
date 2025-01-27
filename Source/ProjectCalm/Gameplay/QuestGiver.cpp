// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestGiver.h"
#include "ProjectCalm/UI/Dialogue.h"
#include "ProjectCalm/Gameplay/Pickup.h"
#include "ProjectCalm/Game/Quests/QuestDetails.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/ArrowComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

AQuestGiver::AQuestGiver()
{
    RewardSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("RewardSpawnPoint"));
    if (RewardSpawnPoint != nullptr && RootComponent != nullptr) {RewardSpawnPoint->SetupAttachment(RootComponent);}

    Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("ArrowComponent"));
    if (Arrow != nullptr && RewardSpawnPoint != nullptr) {Arrow->SetupAttachment(RewardSpawnPoint);}
}

// Called when the game starts or when spawned
void AQuestGiver::BeginPlay()
{
	Super::BeginPlay();

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
	CHECK_NULLPTR_RET(GameMode, LogGameMode, "AQuestGiver:: Failed to get PCGameMode!");
    GameMode->OnQuestUpdated.AddDynamic(this, &AQuestGiver::OnQuestUpdated);
}

void AQuestGiver::HandleInteraction()
{
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
	CHECK_NULLPTR_RET(GameMode, LogGameMode, "AQuestGiver:: Failed to get PCGameMode!");
    UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogActor, "AQuestGiver:: Failed to get PCGameInstance!");

    FDialogue Dialogue = FDialogue(DisplayName, Portrait, DefaultDialogue);
    
    if (CurrentQuestID != 0)
    {
        FQuestDetails CurrentQuest = GameMode->GetQuestDetails(CurrentQuestID);

        if (!CurrentQuest.IsActive() && GameMode->StartQuest(CurrentQuestID))
        {
            Dialogue.Strings = CurrentQuest.Stages[0].IntroDialogue;
            GameMode->MarkIntroDialoguePlayed(CurrentQuest.QuestID, 0);
        }
        else if (CurrentQuest.IsActive() && !CurrentQuest.IsComplete())
        {
            if (!CurrentQuest.Stages[CurrentQuest.ActiveStageIdx].bIntroPlayed && !CurrentQuest.Stages[CurrentQuest.ActiveStageIdx].IntroDialogue.IsEmpty())
            {
                Dialogue.Strings = CurrentQuest.Stages[CurrentQuest.ActiveStageIdx].IntroDialogue;
                GameMode->MarkIntroDialoguePlayed(CurrentQuest.QuestID, CurrentQuest.ActiveStageIdx);
            }
            else if (!CurrentQuest.Stages[CurrentQuest.ActiveStageIdx].OngoingDialogue.IsEmpty())
            {
                Dialogue.Strings = CurrentQuest.Stages[CurrentQuest.ActiveStageIdx].OngoingDialogue;
            }
        }
        else if (CurrentQuest.IsActive() && CurrentQuest.IsComplete())
        {
            CurrentQuestID = CurrentQuest.NextQuestID;
            HandleInteraction();
            return;
        }
    }

    GameInstance->LoadDialogueBox(Dialogue);
}

uint32 AQuestGiver::SpawnRewards(TArray<uint32> ItemIDs)
{
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RETVAL(GameMode, LogGameMode, "QuestGiver:: PCGameMode not found!", 0);

    FVector SpawnLocation = RewardSpawnPoint == nullptr ? GetActorLocation() : RewardSpawnPoint->GetComponentLocation();
    FRotator SpawnRotation = RewardSpawnPoint == nullptr ? GetActorRotation() : RewardSpawnPoint->GetComponentRotation();
    uint32 RewardsSpawned{0};
    for (uint32 ItemID : ItemIDs)
    {
        if (GameMode->SpawnPickup(ItemID, SpawnLocation, SpawnRotation) != nullptr) {RewardsSpawned++;}
        else {UE_LOG(LogInteractable, Warning, TEXT("QuestGiver:: No Pickup spawned by GameMode!"));}
    }

    return RewardsSpawned;
}

void AQuestGiver::Interact(APlayerCharacter* InteractingPlayer)
{
	CHECK_NULLPTR_RET(InteractingPlayer, LogPlayerCharacter, "AQuestGiver:: Interact was not passed a valid PlayerCharacter!");
    
    Super::Interact(InteractingPlayer);

    HandleInteraction();
}

void AQuestGiver::OnQuestUpdated(FQuestDetails QuestDetails)
{
    if (QuestDetails.QuestID != CurrentQuestID || QuestDetails.ActiveStageIdx < 0) {return;}
    
    while (CurrentStageIdx < (uint32)QuestDetails.ActiveStageIdx)
    {
        if (QuestDetails.Stages[CurrentStageIdx].bIsComplete) {SpawnRewards(QuestDetails.Stages[CurrentStageIdx].EquipmentRewards);}
        CurrentStageIdx++;
    }
    
    if (QuestDetails.IsComplete())
    {
        CurrentQuestID = QuestDetails.NextQuestID;
        CurrentStageIdx = 0;
    }
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialRock.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"

#ifdef PC_DEBUG_LOGS
	#define LOCAL_DEBUG_LOGS
#endif

#define SPECIAL_ROCK_QUEST_ID 2


ASpecialRock::ASpecialRock()
{
    InteractionLabel = FString("Grab");
	ActiveCollisionType = ECollisionEnabled::QueryOnly;
	InactiveCollisionType = ECollisionEnabled::NoCollision;

    if (InteractionCollision != nullptr)
	{
        SetCollisionProfile("BlockAllDynamic");
		SetCollisionEnabled(false);
        InteractionCollision->SetEnableGravity(false);
        InteractionCollision->SetSimulatePhysics(false);
	}

    if (InteractionMesh != nullptr)
	{
        InteractionMesh->SetCollisionProfileName("BlockAllDynamic");
        InteractionMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
        InteractionMesh->SetEnableGravity(false);
        InteractionCollision->SetSimulatePhysics(false);
	}
}

void ASpecialRock::BeginPlay()
{
    Super::BeginPlay();

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogInteractable, "SpecialRock:: Could not get Game Mode!");

    GameMode->OnQuestUpdated.AddDynamic(this, &ASpecialRock::OnQuestUpdated);
    
    UpdateRock();
}

void ASpecialRock::Interact(APlayerCharacter* InteractingPlayer)
{
    if (IsAtLocation(HomeLocation) && TargetLocation == FVector::ZeroVector) {return;}

    Super::Interact(InteractingPlayer);

    switch (TimesInteracted)
    {
    case 1:
        InteractionLabel = FString("Grab?");
        break;
    case 2:
        InteractionLabel = FString("Grab!");
        break;
    case 3:
        InteractionLabel = FString("Touch");
        break;    
    default:
        break;
    }

    if (TargetLocation == FVector::ZeroVector || IsAtLocation(TargetLocation))
    {
        TeleportTo(HomeLocation, HomeRotation);
    }
    else {TeleportTo(TargetLocation, TargetRotation);}
    
    // @todo: play fx    
    
    UpdateRock();
}

void ASpecialRock::UpdateRock()
{
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogInteractable, "SpecialRock:: Could not get Game Mode!");

    GameMode->SetSpecialRockLocation(GetActorLocation());

    // FQuestDetails SpecialRockQuest = GameMode->GetQuestDetails(SPECIAL_ROCK_QUEST_ID);
    // if (InteractionCollision != nullptr)
    // {
    //     InteractionCollision->SetCollisionEnabled((SpecialRockQuest.IsActive() && !SpecialRockQuest.IsComplete()) ? ActiveCollisionType : InactiveCollisionType);
    // }
}

bool ASpecialRock::IsAtLocation(FVector TestLocation) const
{
    return FVector::Distance(GetActorLocation(), TestLocation) <= 200;
}

FString ASpecialRock::GetInteractionLabel() const
{
    if (IsAtLocation(HomeLocation) && TargetLocation == FVector::ZeroVector) {return FString();}
    return Super::GetInteractionLabel();
}

void ASpecialRock::OnQuestUpdated(FQuestDetails QuestDetails)
{
    if (QuestDetails.bIsHidden || QuestDetails.IsComplete()) {return;}

    if (QuestDetails.QuestID == SPECIAL_ROCK_QUEST_ID && QuestDetails.ActiveStageIdx == 0)
    {
        SetCollisionEnabled(true);
    }

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogInteractable, Display, TEXT("SpecialRock:: OnQuestUpdated called for quest %s"), *(QuestDetails.Name.ToString()));
#endif
    
    FStageDetails ActiveStage = QuestDetails.Stages[QuestDetails.ActiveStageIdx];
    
#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogInteractable, Display, TEXT("SpecialRock:: Stage: %s"), *(ActiveStage.StageDescription));
#endif

    for (FObjectiveDetails Objective : ActiveStage.Objectives)
    {
        if (!Objective.bIsComplete)
        {
    
#ifdef LOCAL_DEBUG_LOGS
        UE_LOG(LogInteractable, Display, TEXT("SpecialRock:: Objective Location: %s"), *(Objective.TargetLocation.ToCompactString()));
#endif

            TargetLocation = Objective.TargetLocation;
            TargetRotation = Objective.TargetRotation;
            return;
        }
    }
}

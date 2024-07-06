// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


void AInteractableActor::Interact(APlayerCharacter *InteractingPlayer)
{
	if (InteractingPlayer != nullptr) {PlayerCharacter = InteractingPlayer;}

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");
    GameMode->OnInteract.Broadcast(this);
}

void AInteractableActor::SetCollisionEnabled(bool bValue)
{
    if (InteractionCollision != nullptr) {InteractionCollision->SetCollisionEnabled(bValue ? ActiveCollisionType : InactiveCollisionType);}
}

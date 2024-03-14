// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


void AInteractableActor::Interact(APlayerCharacter *InteractingPlayer)
{
	if (InteractingPlayer != nullptr) {PlayerCharacter = InteractingPlayer;}
}

void AInteractableActor::SetCollisionEnabled(bool bValue)
{
    if (InteractionCollision != nullptr) {InteractionCollision->SetCollisionEnabled(bValue ? ActiveCollisionType : InactiveCollisionType);}
}

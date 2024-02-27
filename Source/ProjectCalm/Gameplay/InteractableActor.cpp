// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"

DEFINE_LOG_CATEGORY(LogInteractable)


void AInteractableActor::Interact(APlayerCharacter* InteractingPlayer)
{
	if (InteractingPlayer != nullptr) {PlayerCharacter = InteractingPlayer;}
}

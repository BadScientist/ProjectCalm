// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractableActor.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


AInteractableActor::AInteractableActor()
{    
    InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
    if (InteractionCollision != nullptr)
    {
        SetRootComponent(InteractionCollision);
        SetCollisionProfile("BlockAllDynamic");
    }

    InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractionMesh"));
    if (InteractionMesh != nullptr && InteractionCollision != nullptr) {InteractionMesh->SetupAttachment(InteractionCollision);}
}

void AInteractableActor::Interact(APlayerCharacter* InteractingPlayer)
{
	if (InteractingPlayer != nullptr) {PlayerCharacter = InteractingPlayer;}
    TimesInteracted++;

    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    CHECK_NULLPTR_RET(GameMode, LogQuest, "QuestObjective:: No PCGameMode found!");
    GameMode->OnInteract.Broadcast(this);

    if (InteractionSound != nullptr) {UGameplayStatics::PlaySoundAtLocation(this, InteractionSound, GetActorLocation());}
}

void AInteractableActor::SetCollisionEnabled(bool bValue)
{
    if (InteractionCollision != nullptr) {InteractionCollision->SetCollisionEnabled(bValue ? ActiveCollisionType : InactiveCollisionType);}
}

void AInteractableActor::SetCollisionProfile(FName ProfileName)
{
    CHECK_NULLPTR_RET(InteractionCollision, LogInteractable, "InteractableActor:: No InteractionCollision!");
    InteractionCollision->SetCollisionProfileName(ProfileName);
    InteractionCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

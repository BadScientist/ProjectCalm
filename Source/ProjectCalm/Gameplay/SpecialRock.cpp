// Fill out your copyright notice in the Description page of Project Settings.


#include "SpecialRock.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Components/BoxComponent.h"


ASpecialRock::ASpecialRock()
{
    InteractionLabel = FString("Grab");
	ActiveCollisionType = ECollisionEnabled::QueryOnly;
	InactiveCollisionType = ECollisionEnabled::NoCollision;

    if (InteractionCollision != nullptr)
	{
        SetCollisionProfile("BlockAllDynamic");
		InteractionCollision->SetCollisionEnabled(ActiveCollisionType);
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
    if (GameMode != nullptr) {GameMode->SetSpecialRockLocation(GetActorLocation());}
}

void ASpecialRock::Interact(APlayerCharacter *InteractingPlayer)
{
    if (TeleportLocations.IsEmpty()) {return;}

    Super::Interact(InteractingPlayer);
    
    // @todo: play fx

    TeleportTo(TeleportLocations[0].Location, TeleportLocations[0].Rotation);
    TeleportLocations.RemoveAt(0);
    
    AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
    if (GameMode != nullptr) {GameMode->SetSpecialRockLocation(GetActorLocation());}

    if (TeleportLocations.IsEmpty() && InteractionCollision != nullptr) {InteractionCollision->SetCollisionEnabled(InactiveCollisionType);}
}

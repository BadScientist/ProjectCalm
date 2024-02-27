// Fill out your copyright notice in the Description page of Project Settings.


#include "Proprietor.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AProprietor::AProprietor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InteractionLabel = FString("Talk");

    ProprietorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProprietorMesh"));
    if (ProprietorMesh != nullptr) {SetRootComponent(ProprietorMesh);}

    ProprietorCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("ProprietorCollision"));
    if (ProprietorCollision != nullptr) {ProprietorCollision->SetupAttachment(ProprietorMesh);}
}

// Called when the game starts or when spawned
void AProprietor::BeginPlay()
{
	Super::BeginPlay();
    
    SetCollisionEnabled(false);
}

void AProprietor::Interact(APlayerCharacter* InteractingPlayer)
{
    Super::Interact(InteractingPlayer);

	CHECK_NULLPTR_RET(InteractingPlayer, LogPlayerCharacter, "AProprietor:: Interact was not passed a valid PlayerCharacter!");

    InteractingPlayer->NotifyPlayer(FString("Suzanne the Proprietor: All your base are belong to us!"));    
}

void AProprietor::SetCollisionEnabled(bool bValue)
{
    if (ProprietorCollision != nullptr) {ProprietorCollision->SetCollisionEnabled(bValue ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision);}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Proprietor.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


// Sets default values
AProprietor::AProprietor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

    InteractionLabel = FString("Talk");
    DisplayName = FName("Suzanne the Proprietor");
    DefaultDialogue.Add(FString("I have nothing for you. Begone!"));

    InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
    if (InteractionCollision != nullptr)
    {
        SetRootComponent(InteractionCollision);
        InteractionCollision->SetCollisionProfileName("OverlapAllDynamic");
        InteractionCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
        InteractionCollision->SetEnableGravity(false);
    }

    InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ProprietorMesh"));
    if (InteractionMesh != nullptr)
    {
        InteractionMesh->SetupAttachment(InteractionCollision);
        InteractionMesh->SetCollisionProfileName("BlockAllDynamic");
        InteractionMesh->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
        InteractionMesh->SetEnableGravity(false);
    }
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

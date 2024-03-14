// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


APickup::APickup()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionLabel = FString("Take");
	ActiveCollisionType = ECollisionEnabled::QueryAndPhysics;
	InactiveCollisionType = ECollisionEnabled::PhysicsOnly;

    InteractionCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionCollision"));
    if (InteractionCollision != nullptr)
	{
		SetRootComponent(InteractionCollision);
        InteractionCollision->SetCollisionProfileName("BlockAllDynamic");
		InteractionCollision->SetCollisionEnabled(InactiveCollisionType);
        InteractionCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
        InteractionCollision->SetEnableGravity(true);
	}

    InteractionMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
    if (InteractionMesh != nullptr)
	{
		InteractionMesh->SetupAttachment(InteractionCollision);
        InteractionMesh->SetCollisionProfileName("NoCollision");
        InteractionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        InteractionMesh->SetEnableGravity(false);
	}
}

void APickup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	float TimeAlive = GetWorld()->GetTimeSeconds() - SpawnTime;

	float Alpha = TimeAlive/Duration;
	float NewScale = 1.0f;
	float NewYaw = 0.0f;
	float NewZ = 0.0f;

	if (Alpha < 0.5f)
	{
		NewZ = FMath::Lerp(0, BounceHeight, 2 * Alpha);
		NewYaw = FMath::Lerp(0, 360, 2 * Alpha);
		NewScale = FMath::Lerp(StartScale, 1, 2 * Alpha);
	}
	else if (Alpha < 1.0f)
	{
		NewZ = FMath::Lerp(BounceHeight, 0, FMath::Clamp(2 * (Alpha - 0.5f), 0.0f, 1.0f));
	}
	else
	{
		if (InteractionCollision != nullptr) {InteractionCollision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);}

		SetActorTickEnabled(false);
	}

	if (InteractionMesh != nullptr)
	{
		InteractionMesh->SetRelativeScale3D(FVector(NewScale));
		InteractionMesh->SetRelativeRotation(FRotator(0.0f, NewYaw, 0.0f));
		InteractionMesh->SetRelativeLocation(FVector(0.0f, 0.0f, NewZ));
	}
}

void APickup::Interact(APlayerCharacter *InteractingPlayer)
{
	CHECK_NULLPTR_RET(InteractingPlayer, LogPlayerCharacter, "APickup:: Interact was not passed a valid PlayerCharacter!");

	if (Items.Num() > InteractingPlayer->GetInventorySlotsRemaining())
	{
		InteractingPlayer->NotifyPlayer(FString("Not enough room in inventory!"));
		return;
	}

	for (UItemData* Item : Items)
	{
		FString Response;
		if (!(InteractingPlayer->AddItem(Item) && Item->Activate(GetWorld(), Response)))
		{
			AbortPickup(InteractingPlayer);
			return;
		}
	}

	Destroy();
}

void APickup::BeginPlay()
{
	Super::BeginPlay();

    if (InteractionMesh != nullptr)
	{
		InteractionMesh->SetRelativeScale3D(FVector(StartScale));
	}
	
	SpawnTime = GetWorld()->GetTimeSeconds();
}

void APickup::AbortPickup(APlayerCharacter *InteractingPlayer)
{
	FString Response;
	for (UItemData* Item : Items)
	{
		if (Item->GetIsEquipped()) {Item->Activate(GetWorld(), Response);}
		InteractingPlayer->RemoveItem(Item);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "Pickup.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Utilities/LogMacros.h"

APickup::APickup()
{
	InteractionLabel = FString("Take");
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

void APickup::AbortPickup(APlayerCharacter* InteractingPlayer)
{
	UE_LOG(LogTemp, Error, TEXT("Pickup:: AbortPickup()"));
	FString Response;
	for (UItemData* Item : Items)
	{
		if (Item->GetIsEquipped()) {Item->Activate(GetWorld(), Response);}
		InteractingPlayer->RemoveItem(Item);
	}
}

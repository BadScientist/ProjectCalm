// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCalmGameMode.h"
#include "Inventory/ItemData.h"

#include "UObject/ConstructorHelpers.h"

void AProjectCalmGameMode::BeginPlay()
{
	for (TSoftObjectPtr<UItemData> Item : ItemDataContainer)
	{
		Item.LoadSynchronous();
		if (Item.IsValid())
		{
			UItemData* ItemData = Item.Get();
			ItemDictionary.Add(ItemData->GetID(), ItemData);
		}
	}

	ItemDataContainer.Empty();
}

AProjectCalmGameMode::AProjectCalmGameMode()
    : Super()
{
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (Start)"));
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (End)"));
}

int32 AProjectCalmGameMode::GenerateInstanceID()
{
	int32 Value = NextInstanceID;
	NextInstanceID++;
    return Value;
}

UItemData* AProjectCalmGameMode::GetItemDataFromID(int32 ItemID)
{
	if (UItemData** Data = ItemDictionary.Find(ItemID)) {return *Data;}
    return nullptr;
}

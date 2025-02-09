#include "ItemManager.h"
#include "ProjectCalm/Inventory/ItemData.h"
#include "ProjectCalm/Gameplay/Pickup.h"
#include "ProjectCalm/Utilities/LogMacros.h"


UItemManager::UItemManager()
{
}

void UItemManager::BeginPlay()
{
	Super::BeginPlay();
	
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

UItemData* UItemManager::GetItemDataFromID(int32 ItemID)
{
	UItemData** Data = ItemDictionary.Find(ItemID);
	CHECK_NULLPTR_RETVAL(Data, LogItemData, "ItemManager:: No ItemData found with the given ID!", nullptr);

	return *Data;
}

APickup* UItemManager::SpawnPickup(int32 ItemID, FVector InLocation, FRotator InRotation)
{
    return SpawnPickup(GetItemDataFromID(ItemID), InLocation, InRotation);
}

APickup* UItemManager::SpawnPickup(UItemData* ItemData, FVector InLocation, FRotator InRotation)
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogLevel, "ItemManager:: Could not get World!", nullptr);
	CHECK_NULLPTR_RETVAL(ItemData, LogItemData, "ItemManager:: SpawnPickup was not passed valid ItemData!", nullptr);
	APickup* Pickup = World->SpawnActor<APickup>(InLocation, InRotation);

	CHECK_NULLPTR_RETVAL(Pickup, LogInteractable, "ItemManager:: SpawnActor failed!", nullptr);
	Pickup->Setup(ItemData);
	return Pickup;
}

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
	if (UItemData** Data = ItemDictionary.Find(ItemID)) {return *Data;}
    return nullptr;
}

APickup* UItemManager::SpawnPickup(int32 ItemID, FVector InLocation, FRotator InRotation)
{
    return SpawnPickup(GetItemDataFromID(ItemID), InLocation, InRotation);
}

APickup* UItemManager::SpawnPickup(UItemData* ItemData, FVector InLocation, FRotator InRotation)
{
	CHECK_NULLPTR_RETVAL(ItemData, LogItemData, "ItemManager:: SpawnPickup was not passed valid ItemData!", nullptr);
	APickup* Pickup = GetWorld()->SpawnActor<APickup>(InLocation, InRotation);
	if (Pickup != nullptr) {Pickup->Setup(ItemData);}
	return Pickup;
}

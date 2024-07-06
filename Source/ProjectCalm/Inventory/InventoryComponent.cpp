// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"
#include "ItemData.h"
#include "ProjectCalm/Game/ProjectCalmGameMode.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#define MAX_ITEMS 12


UInventoryComponent::UInventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	Inventory.Init(nullptr, 12);	
}

bool UInventoryComponent::AddItem(int32 ItemID)
{
	AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
	CHECK_NULLPTR_RETVAL(GameMode, LogActorComponent, "InventoryComponent:: ProjectCalmGameMode not found!", false);

    return AddItem(GameMode->GetItemDataFromID(ItemID));
}

bool UInventoryComponent::AddItem(UItemData *Item)
{
	if (Num >= MAX_ITEMS) {return false;}

    return AddItem(Item, GetFirstEmptySlot());
}

bool UInventoryComponent::AddItem(UItemData *Item, int32 Index)
{
	if (Index < 0) {return false;}
	if (Inventory[Index] != nullptr) {return false;}

    Inventory[Index] = Item;
	Num++;

	return true;
}

bool UInventoryComponent::RemoveItem(int32 Index)
{
	if (Inventory[Index] == nullptr) {return false;}

	Inventory[Index] = nullptr;
	Num--;

	return true;
}

bool UInventoryComponent::RemoveItem(UItemData* Item)
{
	int32 i = 0;
    for (UItemData* InventoryItem : Inventory)
	{
		if (InventoryItem == Item)
		{
			return RemoveItem(i);
		}
		i++;
	}
	return false;
}

void UInventoryComponent::SwapItems(int32 FirstIndex, int32 SecondIndex)
{
	UItemData* FirstItem = Inventory[FirstIndex];
	Inventory[FirstIndex] = Inventory[SecondIndex];
	Inventory[SecondIndex] = FirstItem;
}

int32 UInventoryComponent::GetNumRemaining()
{
    return MAX_ITEMS - Num;
}

int32 UInventoryComponent::GetFirstEmptySlot()
{
	for (int32 i = 0; i < MAX_ITEMS; i++)
	{
		if (Inventory[i] == nullptr) {return i;}
	}
	
	return -1;
}

void UInventoryComponent::GetInventory(TArray<UItemData *> &OutInventory)
{
	OutInventory = Inventory;
	UE_LOG(LogTemp, Warning, TEXT("Inventory.Num == %i, OutInventory.Num == %i"), Inventory.Num(), OutInventory.Num());
}

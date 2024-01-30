// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"

class UItemData;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	TArray<UItemData*> Inventory;
	int32 Num{0};

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

public:
	bool AddItem(int32 ItemID);
	bool AddItem(UItemData* Item);
	bool AddItem(UItemData* Item, int32 Index);
	bool RemoveItem(int32 Index);
	void SwapItems(int32 FirstIndex, int32 SecondIndex);
	int32 GetNum(){return Num;};
	int32 GetFirstEmptySlot();

	void GetInventory(TArray<UItemData*>& OutInventory);
		
};

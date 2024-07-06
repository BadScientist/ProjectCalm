// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ItemManager.generated.h"

// class UDataTable;
class UItemData;
class APickup;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UItemManager : public UActorComponent
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = ItemData)
	TArray<TSoftObjectPtr<UItemData>> ItemDataContainer;
	UPROPERTY(VisibleAnywhere, Category = ItemData)
	TMap<int32, UItemData*> ItemDictionary;    

public:
	UItemManager();
	
	virtual void BeginPlay() override;

	UItemData* GetItemDataFromID(int32 ItemID);
	APickup* SpawnPickup(int32 ItemID, FVector InLocation, FRotator InRotation);
	APickup* SpawnPickup(UItemData* ItemData, FVector InLocation, FRotator InRotation);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemDetails.h"
#include "ItemData.generated.h"

class AEquipment;
class IEquipmentInterface;
class APickup;
enum EEquipReply;


/**
 * 
 */
UCLASS(BLueprintType)
class PROJECTCALM_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UItemData(const FObjectInitializer& ObjectInitializer);

private:
    UPROPERTY(EditAnywhere, Category = BaseData)
    int32 ItemID{0x0000};
    UPROPERTY(EditAnywhere, Category = Details)
    FItemDetails ItemDetails;
    UPROPERTY(EditAnywhere, Category = Assets)
    UTexture2D* Icon{nullptr};
    UPROPERTY(EditAnywhere, Category = Assets)
    UStaticMesh* Mesh{nullptr};
    UPROPERTY(EditAnywhere, Category = Assets)
    TSubclassOf<AActor> ItemBPClass{nullptr};

    AActor* EquippedInstance;
    int32 InstanceID{0};

    void AssignInstanceID(IEquipmentInterface* Equipment);

public:
    int32 GetID() {return ItemID;};
    UTexture2D* GetInventoryIcon() {return Icon;};
    FItemDetails GetItemDetails() {return ItemDetails;}
    UStaticMesh* GetMesh() {return Mesh;}

    bool GetIsEquipped();

    virtual bool Activate(UWorld* InWorld, FString& OutFailureResponse);
	
};
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "ItemDetails.h"
#include "ItemData.generated.h"

class AEquipment;
class IEquipmentInterface;
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
    UPROPERTY(EditAnywhere, Category = Images)
    UTexture2D* Icon{nullptr};
    UPROPERTY(EditAnywhere, Category = Blueprint)
    TSubclassOf<AActor> ItemBPClass{nullptr};

    AActor* EquippedInstance;

public:
    int32 GetID() {return ItemID;};
    UTexture2D* GetInventoryIcon() {return Icon;};
    FItemDetails GetItemDetails();

    bool GetIsEquipped();

    virtual bool Activate(UWorld* InWorld, FString& OutFailureResponse);
	
};
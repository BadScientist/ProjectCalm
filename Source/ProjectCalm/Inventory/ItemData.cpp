// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemData.h"
#include "EquipmentInterface.h"
#include "EquipReply.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

UItemData::UItemData(const FObjectInitializer &ObjectInitializer)
{
    
}

FItemDetails UItemData::GetItemDetails()
{
    return ItemDetails;
}

bool UItemData::GetIsEquipped()
{
    if (EquippedInstance != nullptr)
    {
        if (EquippedInstance->IsValidLowLevel() && !EquippedInstance->IsActorBeingDestroyed()) {return true;}
        else {EquippedInstance = nullptr;}
    }
    return false;
}

bool UItemData::Activate(UWorld* InWorld, FString& OutFailureResponse)
{
    if (GetIsEquipped())
    {
        if (IEquipmentInterface* Interface = Cast<IEquipmentInterface>(EquippedInstance)){Interface->Unequip();}
        EquippedInstance = nullptr;
        return true;
    }

    EEquipReply EquipReply = EEquipReply::NO_REPLY;
    bool bResult = false;

    if (ItemBPClass != nullptr && ItemBPClass->IsValidLowLevel())
    {
        if (AActor* SpawnedActor = InWorld->SpawnActor<AActor>(ItemBPClass))
        {
            if (IEquipmentInterface* SpawnedInstance = Cast<IEquipmentInterface>(SpawnedActor))
            {
                APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(InWorld);
                CHECK_NULLPTR_RETVAL(PlayerCharacter, LogPlayerCharacter, "ItemData::No PlayerCharacter found!", false);

                EquipReply = SpawnedInstance->Equip(PlayerCharacter);
                if (EquipReply == EEquipReply::SUCCESS)
                {
                    bResult = true;
                    EquippedInstance = SpawnedActor;
                }
                else {SpawnedActor->Destroy();}
            }
        }
    }

    OutFailureResponse = EquipReply::EnumToString(EquipReply);
    return bResult;
}

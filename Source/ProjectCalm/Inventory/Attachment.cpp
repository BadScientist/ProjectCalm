// Fill out your copyright notice in the Description page of Project Settings.


#include "Attachment.h"
#include "EquipReply.h"
#include "EquipperInterface.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

EEquipReply AAttachment::Equip(APlayerCharacter *OwningCharacter)
{
    return Equip_Internal(OwningCharacter);
}

EEquipReply AAttachment::Equip_Internal(AActor *OwningActor)
{
    if (IEquipperInterface* Equipper = Cast<IEquipperInterface>(OwningActor))
    {
        if (Equipper->AttachEquipment(this, TargetSocket)) {return EEquipReply::SUCCESS;}
    }
    
    return EEquipReply::FAILED_DEFAULT;
}

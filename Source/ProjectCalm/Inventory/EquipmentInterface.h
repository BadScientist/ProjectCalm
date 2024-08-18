// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

class APlayerCharacter;
enum EEquipReply;

UINTERFACE(MinimalAPI)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API IEquipmentInterface
{
	GENERATED_BODY()

public:
	virtual EEquipReply Equip(APlayerCharacter* OwningCharacter) = 0;
	virtual void Unequip() = 0;
	virtual void SetInstanceID(int32 InID) = 0;
	virtual int32 GetInstanceID() = 0;
};

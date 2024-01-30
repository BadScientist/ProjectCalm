// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipperInterface.generated.h"

class IEquipmentInterface;


UINTERFACE(MinimalAPI)
class UEquipperInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API IEquipperInterface
{
	GENERATED_BODY()

public:
	virtual bool AttachEquipment(IEquipmentInterface* Equipment, FName SocketName) = 0;
	virtual void RemoveEquipment(IEquipmentInterface* Equipment) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "EquipmentInterface.generated.h"

UINTERFACE(MinimalAPI)
class UEquipmentInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API IEquipmentInterface
{
	GENERATED_BODY()

public:
	virtual void Equip(AActor* OwningActor, FName SocketName) = 0;
};

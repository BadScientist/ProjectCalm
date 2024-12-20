// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SpawnRegionInterface.generated.h"

class UPopupWidget;


UINTERFACE(MinimalAPI)
class USpawnRegionInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API ISpawnRegionInterface
{
	GENERATED_BODY()

public:
	virtual FVector GetSize() const = 0;
	virtual FVector GetRegionLocation() const = 0;
	virtual FRotator GetRegionRotation() const = 0;
	virtual void SetSize(FVector InSize) = 0;
	virtual void SetRegionLocation(FVector InLocation) = 0;
	virtual void SetRegionRotation(FRotator InRotation) = 0;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "QuestResult.generated.h"

class UItemData;


UCLASS()
class PROJECTCALM_API AQuestResult : public AActor
{
	GENERATED_BODY()

    UItemData* EquipmentReward{nullptr};
	
public:
	// Sets default values for this actor's properties
	AQuestResult();

};

// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectCalmGameMode.generated.h"

class UItemData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMakeNoiseDelegate, AActor*, ResponsibleActor, float, NoiseLevel);


UCLASS(minimalapi)
class AProjectCalmGameMode : public AGameModeBase
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = ItemData)
	TArray<TSoftObjectPtr<UItemData>> ItemDataContainer;
	UPROPERTY(VisibleAnywhere, Category = ItemData)
	TMap<int32, UItemData*> ItemDictionary;

	int32 NextInstanceID{0x0001};

protected:
	virtual void BeginPlay() override;

public:
	AProjectCalmGameMode();

	FOnMakeNoiseDelegate OnMakeNoise;

	int32 GenerateInstanceID();
	UItemData* GetItemDataFromID(int32 ItemID);
};




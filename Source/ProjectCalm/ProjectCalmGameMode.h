// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ProjectCalmGameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMakeNoiseDelegate, AActor*, ResponsibleActor, float, NoiseLevel);

UCLASS(minimalapi)
class AProjectCalmGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AProjectCalmGameMode();

	FOnMakeNoiseDelegate OnMakeNoise;
};




// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

#include "ProjectCalm/Game/ProjectCalmGameInstance.h"
#include "ProjectCalm/Game/ProjectCalmGameMode.h"


class PCGameStatics
{
public:
    static UProjectCalmGameInstance* GetPCGameInstance(const UObject* WorldContextObject)
    {
        return Cast<UProjectCalmGameInstance>(UGameplayStatics::GetGameInstance(WorldContextObject));
    }

    static AProjectCalmGameMode* GetPCGameMode(const UObject* WorldContextObject)
    {
        return Cast<AProjectCalmGameMode>(UGameplayStatics::GetGameMode(WorldContextObject));
    }

    static void PauseGame(const UObject* WorldContextObject)
    {
        AProjectCalmGameMode* GameMode = GetPCGameMode(WorldContextObject);
        if (GameMode != nullptr) {GameMode->PauseGame();}
    }

    static void UnpauseGame(const UObject* WorldContextObject)
    {
        AProjectCalmGameMode* GameMode = GetPCGameMode(WorldContextObject);
        if (GameMode != nullptr) {GameMode->UnpauseGame();}
    }
};
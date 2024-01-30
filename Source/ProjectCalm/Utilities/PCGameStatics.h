#pragma once

#include "CoreMinimal.h"
#include "Kismet/GameplayStatics.h"

#include "ProjectCalm/ProjectCalmGameInstance.h"
#include "ProjectCalm/ProjectCalmGameMode.h"


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
};
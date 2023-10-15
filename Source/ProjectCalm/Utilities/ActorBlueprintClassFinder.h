#pragma once

#include "CoreMinimal.h"

#include "ActorBlueprintClassFinder.generated.h"


UCLASS()
class PROJECTCALM_API AActorBlueprintClassFinder : public AActor
{
	GENERATED_BODY()

public:
    AActorBlueprintClassFinder(){};
    AActorBlueprintClassFinder(FString Path);

    TSubclassOf<AActor> Class = nullptr;

};

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "PhotoSubjectComponent.generated.h"

class UPhotoSubjectDataComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoSubjectComponent : public UBoxComponent
{
	GENERATED_BODY()

public:
    UPhotoSubjectComponent();

public:
    bool Spawn();
    bool Despawn(FVector PlayerLocation, FVector PlayerForwardVector);

};
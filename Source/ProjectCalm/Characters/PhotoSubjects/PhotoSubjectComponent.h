#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"

#include "PhotoSubjectComponent.generated.h"

class UPhotoSubjectDataComponent;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoSubjectComponent : public UBoxComponent
{
	GENERATED_BODY()

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EPhysicalSurface>> ValidSurfaces;

public:
    UPhotoSubjectComponent();

public:
    bool Spawn(float RegionHeight = 0.0f);
    bool Despawn(AActor* Player);

};
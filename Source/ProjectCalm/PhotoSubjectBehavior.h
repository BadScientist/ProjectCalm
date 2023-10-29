#pragma once

#include "PhotoSubjectBehavior.generated.h"

UENUM(BlueprintType)
enum EPhotoSubjectBehavior
{
    IDLE UMETA(DisplayName = "Idle"),
    WANDER UMETA(DisplayName = "Wander"),
    GRAZE UMETA(DisplayName = "Graze")
};


USTRUCT(BlueprintType)
struct FWeightedBehavior
{
	GENERATED_BODY()

    FWeightedBehavior(){};
    FWeightedBehavior(EPhotoSubjectBehavior InBehavior, float InWeight) : Behavior(InBehavior), Weight(InWeight) {};

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EPhotoSubjectBehavior> Behavior;

    /** Represents the relative probability of the behavior being selected*/
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
    float Weight;

};

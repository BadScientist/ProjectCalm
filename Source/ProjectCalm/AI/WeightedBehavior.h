#pragma once

#include "PhotoSubjectBehavior.h"
#include "WeightedBehavior.generated.h"


USTRUCT(BlueprintType)
struct FWeightedBehavior
{
	GENERATED_BODY()

    FWeightedBehavior(){};
    FWeightedBehavior(EPhotoSubjectBehavior InBehavior, float InWeight) : Behavior(InBehavior), Weight(InWeight) {};

    UPROPERTY(EditAnywhere)
    TEnumAsByte<EPhotoSubjectBehavior> Behavior{EPhotoSubjectBehavior::NONE};

    /** Represents the relative probability of the behavior being selected*/
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
    float Weight{1.0f};

    /** Factor the weight is multiplied by each time the behavior is consecutively selected.*/
    UPROPERTY(EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
    float DecayMultiplier{1.0f};

    int32 ConsecutiveSelections{0};

    float GetAdjustedWeight()
    {
        return Weight * pow(DecayMultiplier, ConsecutiveSelections);
    }

};
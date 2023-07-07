#pragma once

#include "WidgetVector.h"
#include "PhotoSubjectPointOfInterest.generated.h"


USTRUCT(BlueprintType)
struct FPhotoSubjectPointOfInterest
{
	GENERATED_BODY()

	FPhotoSubjectPointOfInterest(){};
	FPhotoSubjectPointOfInterest(FName POIName, FVector POIRelativeLocation, float POIScoreValue)
    {
        Name = POIName;
        RelativeLocation = POIRelativeLocation;
        ScoreValue = POIScoreValue;
    };
    
    UPROPERTY(EditAnywhere)
    FName Name;
    UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
    FWidgetVector RelativeLocation = FWidgetVector::Zero();
    UPROPERTY(EditAnywhere)
    float ScoreValue = 1.0f;
    UPROPERTY(EditAnywhere)
    bool IsInLineOfSight = false;
    UPROPERTY(EditAnywhere)
    bool IsOnCamera = false;

    bool IsVisible() {return IsInLineOfSight && IsOnCamera;}
};
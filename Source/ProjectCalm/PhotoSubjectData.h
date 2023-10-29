#pragma once

#include "PhotoSubjectPointOfInterest.h"
#include "PhotoSubjectData.generated.h"


USTRUCT(BlueprintType)
struct FPhotoSubjectData
{
	GENERATED_BODY()

	FPhotoSubjectData(){};
	FPhotoSubjectData(FName SubjectName, FVector SubjectLocation, TArray<FPhotoSubjectPointOfInterest> SubjectPOIs)
    {
        Name = SubjectName;
        Location = SubjectLocation;
        PointsOfInterest.Append(SubjectPOIs);
    };
    
    UPROPERTY(EditAnywhere)
    FName Name;
    UPROPERTY(EditAnywhere)
    FVector Location {FVector::ZeroVector};
    UPROPERTY(EditAnywhere)
    TArray<FPhotoSubjectPointOfInterest> PointsOfInterest;
};
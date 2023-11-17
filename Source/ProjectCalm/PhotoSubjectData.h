#pragma once

#include "PhotoSubjectPointOfInterest.h"
#include "PhotoSubjectName.h"
#include "PhotoSubjectData.generated.h"


USTRUCT(BlueprintType)
struct FPhotoSubjectData
{
	GENERATED_BODY()

	FPhotoSubjectData(){};
	FPhotoSubjectData(ESubjectName SubjectName, FVector SubjectLocation, TArray<FPhotoSubjectPointOfInterest> SubjectPOIs)
    {
        Name = SubjectName;
        Location = SubjectLocation;
        PointsOfInterest.Append(SubjectPOIs);
    };
    
    UPROPERTY(EditAnywhere)
    TEnumAsByte<ESubjectName> Name;
    UPROPERTY(EditAnywhere)
    FVector Location {FVector::ZeroVector};
    UPROPERTY(EditAnywhere)
    TArray<FPhotoSubjectPointOfInterest> PointsOfInterest;
};
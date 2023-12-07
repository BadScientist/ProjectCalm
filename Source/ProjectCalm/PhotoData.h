#pragma once

#include "Engine/TextureRenderTarget2D.h"

#include "PhotoSubjectData.h"
#include "PhotoData.generated.h"


USTRUCT(BlueprintType)
struct FPhotoData
{
	GENERATED_BODY()

	FPhotoData(){};
    
    UPROPERTY(VisibleAnywhere)
    FDateTime TimeTaken;
    
    UPROPERTY(VisibleAnywhere)
    UTextureRenderTarget2D* Image{nullptr};
    
    UPROPERTY(VisibleAnywhere)
    TArray<FPhotoSubjectData> Subjects;
};
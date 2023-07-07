#pragma once

#include "Engine/TextureRenderTarget2D.h"

#include "PhotoSubjectData.h"
#include "PhotoData.generated.h"


USTRUCT(BlueprintType)
struct FPhotoData
{
	GENERATED_BODY()

	FPhotoData(){};
    
    FDateTime TimeTaken;
    UTextureRenderTarget2D* Image = nullptr;
    TArray<FPhotoSubjectData> Subjects;
};
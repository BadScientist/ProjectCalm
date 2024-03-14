#pragma once

#include "FancyText.h"
#include "TextStyle.generated.h"


USTRUCT(BlueprintType)
struct FFancyTextStyle
{
	GENERATED_BODY()

    FFancyTextStyle(){};
    
    UPROPERTY(EditAnywhere)
    FString Name{FString("default")};
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<UFancyText> WidgetClass;
    
};
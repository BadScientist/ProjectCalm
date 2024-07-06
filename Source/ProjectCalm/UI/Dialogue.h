#pragma once

#include "Engine/Texture2D.h"
#include "Dialogue.generated.h"


USTRUCT(BlueprintType)
struct FDialogue
{
	GENERATED_BODY()

    FDialogue(){};
    FDialogue(FName InSpeaker, UTexture2D* InPortrait, TArray<FString> InStrings) : 
        Speaker(InSpeaker), Portrait(InPortrait), Strings(InStrings){};
    
    UPROPERTY(EditAnywhere)
    FName Speaker{"DefaultSpeakerName"};
    
    UPROPERTY(EditAnywhere)
    UTexture2D* Portrait{nullptr};
    
    UPROPERTY(EditAnywhere)
    TArray<FString> Strings;
};
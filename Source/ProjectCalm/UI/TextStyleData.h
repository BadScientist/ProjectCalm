// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"

#include "TextStyle.h"
#include "TextStyleData.generated.h"

class UFancyText;


/**
 * 
 */
UCLASS(BLueprintType)
class PROJECTCALM_API UTextStyleData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
    UTextStyleData(const FObjectInitializer& ObjectInitializer);

private:
    UPROPERTY(EditAnywhere, Category = Styles)
    TArray<FFancyTextStyle> Styles;

    UPROPERTY(EditAnywhere, Category = Styles)
    int32 DefaultStyleIdx{0};

public:
    FFancyTextStyle GetStyleByString(FString StyleName);
    TSubclassOf<UFancyText> GetWidgetClassByString(FString StyleName);
    FFancyTextStyle GetDefaultStyle();
	
};
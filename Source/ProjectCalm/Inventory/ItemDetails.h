#pragma once

#include "CoreMinimal.h"
#include "ItemDetails.generated.h"


USTRUCT()
struct FItemDetails
{
	GENERATED_BODY();

	FItemDetails(){};
	FItemDetails(UTexture2D* InImage, FString InName, FString InDescription) :
        ItemImage(InImage), ItemName(InName), ItemDescription(InDescription) {};

	UPROPERTY(EditAnywhere)
    UTexture2D* ItemImage{nullptr};

	UPROPERTY(EditAnywhere)
	FString ItemName{FString()};

    UPROPERTY(EditAnywhere)
    FString ItemDescription{FString()};

    bool IsValid() {
        UE_LOG(LogTemp, Display, TEXT("%x, %s, %s"), ItemImage, *ItemName, *ItemDescription);
        return (ItemImage != nullptr && ItemName != FString() && ItemDescription != FString());};

};
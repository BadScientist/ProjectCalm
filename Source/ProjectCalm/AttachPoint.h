#pragma once

#include "AttachmentType.h"
#include "AttachPoint.generated.h"


USTRUCT(BlueprintType)
struct FAttachPoint {
	GENERATED_BODY();
	
    UPROPERTY(EditAnywhere)
    FName SlotName;
    UPROPERTY(EditAnywhere)
    TEnumAsByte<AttachmentType> SlotType;
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector RelativeLocation;

    FAttachPoint() {};

    FAttachPoint(FName SlotName, TEnumAsByte<AttachmentType> SlotType, FVector RelativeLocation) : SlotName(SlotName), SlotType(SlotType), RelativeLocation(RelativeLocation) {};
};
#pragma once

#include "CoreMinimal.h"


UENUM(BlueprintType)
enum EEquipReply : uint8
{
    NO_REPLY UMETA(DisplayName = "No Reply"),
    SUCCESS UMETA(DisplayName = "Success"),
    FAILED_DEFAULT UMETA(DisplayName = "Failed"),
    FAILED_NO_CAMERA UMETA(DisplayName = "Failed - No Camera Equipped")
};

class EquipReply
{
public:
    static FString EnumToString(EEquipReply Enum)
    {
        switch (Enum)
        {
        case EEquipReply::NO_REPLY:
            return FString("ERROR");
        case EEquipReply::FAILED_NO_CAMERA:
            return FString("You must have a camera equipped first!");
        
        default:
            return FString();
        }
    }

};
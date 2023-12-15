#pragma once

//#include "CoreMinimal.h"


UENUM(BlueprintType)
enum ESubjectName
{
    NO_NAME UMETA(DisplayName = "Default"),
    DOE UMETA(DisplayName = "Doe"),
    BUCK UMETA(DisplayName = "Buck"),
    WOLF UMETA(DisplayName = "Wolf"),
    CROW UMETA (DisplayName = "Crow")
};

class SubjectName
{
public:
    static const FString SubjectNameEnumToString(ESubjectName InEnum)
    {
        switch (InEnum)
        {
        case ESubjectName::NO_NAME :
            return FString("No Name");
        case ESubjectName::DOE :
            return FString("Doe");
        case ESubjectName::BUCK :
            return FString("Buck");
        case ESubjectName::WOLF :
            return FString("Wolf");
        case ESubjectName::CROW :
            return FString("Crow");
        
        default:
            return FString();
        }
        return FString();
    }
};

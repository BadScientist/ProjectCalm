#pragma once

//#include "CoreMinimal.h"


UENUM(BlueprintType)
enum ESubjectName
{
    TEST UMETA(DisplayName = "Test"),
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
        case ESubjectName::TEST :
            return FString("Test");
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

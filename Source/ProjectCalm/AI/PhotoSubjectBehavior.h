#pragma once

UENUM(BlueprintType)
enum EPhotoSubjectBehavior
{
    NONE                UMETA(DisplayName = "None"),
    RESTART_LAST        UMETA(DisplayName = "Restart Last"),
    IDLE                UMETA(DisplayName = "Idle"),
    WANDER              UMETA(DisplayName = "Wander"),
    GRAZE               UMETA(DisplayName = "Graze"),
    REST                UMETA(DisplayName = "Rest"),
    SEARCH_STATIONARY   UMETA(DisplayName = "Search Stationary"),
    SEARCH_MOBILE       UMETA(DisplayName = "Search Mobile"),
    CHASE               UMETA(DisplayName = "Chase")
};

class PCPhotoSubjectBehavior
{
public:
    static const FString EnumToString(EPhotoSubjectBehavior InEnum)
    {
        switch (InEnum)
        {
        case EPhotoSubjectBehavior::NONE :
            return FString("None");
        case EPhotoSubjectBehavior::RESTART_LAST :
            return FString("Restart Last");
        case EPhotoSubjectBehavior::IDLE :
            return FString("Idle");
        case EPhotoSubjectBehavior::WANDER :
            return FString("Wander");
        case EPhotoSubjectBehavior::GRAZE :
            return FString("Graze");
        case EPhotoSubjectBehavior::REST :
            return FString("Rest");
        case EPhotoSubjectBehavior::SEARCH_STATIONARY :
            return FString("Search Stationary");
        case EPhotoSubjectBehavior::SEARCH_MOBILE :
            return FString("Search Mobile");
        case EPhotoSubjectBehavior::CHASE :
            return FString("Chase");
        
        default:
            return FString("Idle");
        }
        return FString();
    }
};

UENUM(BlueprintType)
enum EPhotoSubjectReaction
{
    IGNORE  UMETA(DisplayName = "Ignore"),
    FLEE    UMETA(DisplayName = "Flee"),
    ATTACK  UMETA(DisplayName = "Attack")
};

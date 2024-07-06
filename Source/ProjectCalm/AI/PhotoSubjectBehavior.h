#pragma once

UENUM(BlueprintType)
enum EPhotoSubjectBehavior
{
    NONE UMETA(DisplayName = "None"),
    RESTART_LAST UMETA(DisplayName = "Restart Last"),
    IDLE UMETA(DisplayName = "Idle"),
    WANDER UMETA(DisplayName = "Wander"),
    GRAZE UMETA(DisplayName = "Graze"),
    REST UMETA(DisplayName = "Rest"),
    SEARCH_STATIONARY UMETA(DisplayName = "Search Stationary"),
    SEARCH_MOBILE UMETA(DisplayName = "Search Mobile"),
    CHASE UMETA(DisplayName = "Chase")
};

class PhotoSubjectBehavior
{
public:
    static const FString PhotoSubjectBehaviorEnumToString(EPhotoSubjectBehavior InEnum)
    {
        switch (InEnum)
        {
        case EPhotoSubjectBehavior::WANDER :
            return FString("Moving About");
        case EPhotoSubjectBehavior::GRAZE :
            return FString("Grazing");
        case EPhotoSubjectBehavior::REST :
            return FString("Resting");
        case EPhotoSubjectBehavior::SEARCH_STATIONARY || EPhotoSubjectBehavior::SEARCH_MOBILE:
            return FString("Searching");
        case EPhotoSubjectBehavior::CHASE :
            return FString("Chasing");
        
        default:
            return FString("Idle");
        }
        return FString();
    }
};

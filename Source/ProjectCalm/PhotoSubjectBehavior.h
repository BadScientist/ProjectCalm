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
    LOOK_AROUND UMETA(DisplayName = "Look Around")
};

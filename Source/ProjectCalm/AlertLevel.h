#pragma once

UENUM(BlueprintType)
enum EAlertLevel
{
    CALM UMETA(DisplayName = "Calm"),
    ALERT UMETA(DisplayName = "Alert"),
    ALARMED UMETA(DisplayName = "Alarmed"),
    AGGRO UMETA(DisplayName = "Aggro")
};
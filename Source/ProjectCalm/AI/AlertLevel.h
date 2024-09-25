#pragma once

UENUM(BlueprintType)
enum EAlertLevel
{
    CALM UMETA(DisplayName = "Calm"),
    ALERT UMETA(DisplayName = "Alert"),
    ALARMED UMETA(DisplayName = "Alarmed"),
    AGGRO UMETA(DisplayName = "Aggro")
};

class PCAlertLevel
{
public:
    static const FString EnumToString(EAlertLevel InEnum)
    {
        switch (InEnum)
        {
        case EAlertLevel::CALM :
            return FString("Calm");
        case EAlertLevel::ALERT :
            return FString("Alert");
        case EAlertLevel::ALARMED :
            return FString("Alarmed");
        case EAlertLevel::AGGRO :
            return FString("Aggro");
        
        default:
            return FString("Calm");
        }
        return FString();
    }
};
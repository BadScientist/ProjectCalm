#include "NotificationComponent.h"
#include "ProjectCalm/UI/NotificationWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "GameFramework/Character.h"


UNotificationComponent::UNotificationComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UNotificationComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
    if (ActiveWidget == nullptr) {return;}

    UWorld* World = GetWorld();
    float TimeActive = World == nullptr ? DisplayTime : World->GetTimeSeconds() - StartTimeStamp;
    ActiveWidget->SetAlpha(FMath::Clamp(1 - (TimeActive - DisplayTime)/FadeTime, 0, 1));
}

void UNotificationComponent::DisplayString(FString InString)
{
    if (ActiveWidget != nullptr)
    {
	    ActiveWidget->DisplayString(InString);
    }

    UWorld* World = GetWorld();
    StartTimeStamp = World == nullptr ? 0.0f : World->GetTimeSeconds();
}

#include "NotificationWidget.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/TextBlock.h"
#include "Components/Image.h"

void UNotificationWidget::DisplayString(FString InString)
{
    CHECK_NULLPTR_RET(NotificationText, LogUserWidget, "NotificationWidget::NotificationText is NULL!");
    NotificationText->SetText(FText::FromString(InString));
}

void UNotificationWidget::SetAlpha(float InAlpha)
{
    CHECK_NULLPTR_RET(NotificationText, LogUserWidget, "NotificationWidget::NotificationText is NULL!");
    NotificationText->SetOpacity(InAlpha);
    CHECK_NULLPTR_RET(NotificationBackground, LogUserWidget, "NotificationWidget::NotificationBackground is NULL!");
    NotificationBackground->SetOpacity(InAlpha);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupWidget.h"
#include "MenuInterface.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"


UPopupWidget::UPopupWidget(const FObjectInitializer& ObjectInitializer) : UMenu(ObjectInitializer)
{
	bIsFocusable = true;
}

void UPopupWidget::CloseMenu()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "PopupWidget:: No MenuInterface!");

    MenuInterface->ClosePopup(this);
}

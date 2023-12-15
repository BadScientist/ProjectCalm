// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupMenu.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"


bool UPopupMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(CloseButton, LogMenuWidget, "PopupMenu:: No CloseButton in Widget Blueprint!", false);
    CloseButton->OnClicked.AddDynamic(this, &UPopupMenu::CloseMenu);

    return true;
}

void UPopupMenu::CloseMenu()
{
    Teardown();
}

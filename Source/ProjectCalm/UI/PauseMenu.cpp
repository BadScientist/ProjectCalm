// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"


bool UPauseMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(QuitButton, LogMenuWidget, "PauseMenu:: No QuitButton in Widget Blueprint!", false);
    QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitToMainMenu);

    return true;
}

void UPauseMenu::QuitToMainMenu()
{
    CHECK_NULLPTR_RET(MenuInterface, LogMenuWidget, "PauseMenu:: No MenuInterface!");

    MenuInterface->QuitToMainMenu();
}

// Copyright 2025 Joseph D Tong aka "BadScientist"

#include "PauseMenu.h"
#include "MenuInterface.h"
#include "OptionsMenu.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


bool UPauseMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(ResumeButton, LogUserWidget, "PauseMenu:: No ResumeButton in Widget Blueprint!", false);
    ResumeButton->OnClicked.AddDynamic(this, &UPauseMenu::OnCloseButtonClicked);
    ResumeButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(QuitButton, LogUserWidget, "PauseMenu:: No QuitButton in Widget Blueprint!", false);
    QuitButton->OnClicked.AddDynamic(this, &UPauseMenu::QuitToMainMenu);
    QuitButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(OptionsButton, LogUserWidget, "PauseMenu:: No OptionsButton in Widget Blueprint!", false);
    OptionsButton->OnClicked.AddDynamic(this, &UPauseMenu::SwitchToOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    return true;
}

void UPauseMenu::QuitToMainMenu()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "PauseMenu:: No MenuInterface!");
    PlayButtonPressedSound(true);
    MenuInterface->QuitToMainMenu();
}

void UPauseMenu::SwitchToOptionsMenu()
{
    PlayButtonPressedSound();

    CHECK_NULLPTR_RET(MenuSwitcher, LogUserWidget, "PauseMenu:: No MenuSwitcher!");
    MenuSwitcher->SetActiveWidget(OptionsMenu);
}

void UPauseMenu::OnCloseButtonClicked()
{
    if (OptionsMenu != nullptr && OptionsMenu->IsAwaitingConfirmation()) {return;}
    Super::OnCloseButtonClicked();
}

void UPauseMenu::SetMenuInterface(IMenuInterface *NewMenuInterface)
{
    Super::SetMenuInterface(NewMenuInterface);

    CHECK_NULLPTR_RET(OptionsMenu, LogUserWidget, "MainGameMenu:: No OptionsMenu!");
    OptionsMenu->SetMenuInterface(NewMenuInterface);
}

void UPauseMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    CHECK_NULLPTR_RET(OptionsMenu, LogUserWidget, "MainGameMenu:: No OptionsMenu!");
    OptionsMenu->SetWidgetSwitcher(MenuSwitcher);
    OptionsMenu->Setup(bIsInteractiveIn);

    PCGameStatics::PauseGame(this);
}

void UPauseMenu::Teardown()
{
    PCGameStatics::UnpauseGame(this);

    Super::Teardown();
}

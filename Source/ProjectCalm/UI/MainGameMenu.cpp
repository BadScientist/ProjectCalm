#include "MainGameMenu.h"
#include "MenuInterface.h"
#include "OptionsMenu.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


bool UMainGameMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(StartButton, LogUserWidget, "MainGameMenu:: No StartButton in Widget Blueprint!", false);
    StartButton->OnClicked.AddDynamic(this, &UMainGameMenu::StartGame);
    StartButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(OptionsButton, LogUserWidget, "MainGameMenu:: No OptionsButton in Widget Blueprint!", false);
    OptionsButton->OnClicked.AddDynamic(this, &UMainGameMenu::SwitchToOptionsMenu);
    OptionsButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(ExitButton, LogUserWidget, "MainGameMenu:: No ExitButton in Widget Blueprint!", false);
    ExitButton->OnClicked.AddDynamic(this, &UMainGameMenu::ExitGame);
    ExitButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    return true;
}

void UMainGameMenu::StartGame()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "MainGameMenu:: No MenuInterface!");
    PlayStartButtonPressedSound();
    MenuInterface->StartGame();
}

void UMainGameMenu::SwitchToOptionsMenu()
{
    PlayButtonPressedSound();

    CHECK_NULLPTR_RET(MenuSwitcher, LogUserWidget, "PauseMenu:: No MenuSwitcher!");
    MenuSwitcher->SetActiveWidget(OptionsMenu);
}

void UMainGameMenu::ExitGame()
{
    PlayButtonPressedSound();
    bIsInteractive = false;

    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "MainGameMenu:: No MenuInterface!");
    MenuInterface->QuitToDesktop();
}

void UMainGameMenu::SetMenuInterface(IMenuInterface *NewMenuInterface)
{
    Super::SetMenuInterface(NewMenuInterface);

    CHECK_NULLPTR_RET(OptionsMenu, LogUserWidget, "MainGameMenu:: No OptionsMenu!");
    OptionsMenu->SetMenuInterface(NewMenuInterface);
}

void UMainGameMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "Menu:: PlayerController is NULL!");

    SetIsFocusable(true);
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

    CHECK_NULLPTR_RET(OptionsMenu, LogUserWidget, "MainGameMenu:: No OptionsMenu!");
    OptionsMenu->SetWidgetSwitcher(MenuSwitcher);
    OptionsMenu->Setup(bIsInteractiveIn);
}

void UMainGameMenu::Teardown()
{
    Super::Teardown();
    
    if (bIsInteractive) {SetGameOnlyControls();}
}

void UMainGameMenu::PlayStartButtonPressedSound()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "Menu:: MenuInterface is NULL!");
    MenuInterface->PlayUISound("UIStartButtonPressed", this, true);
}

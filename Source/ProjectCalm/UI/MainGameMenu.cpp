#include "MainGameMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"


bool UMainGameMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(StartButton, LogMenuWidget, "MainGameMenu:: No StartButton in Widget Blueprint!", false);
    StartButton->OnClicked.AddDynamic(this, &UMainGameMenu::StartGame);

    CHECK_NULLPTR_RETVAL(ExitButton, LogMenuWidget, "MainGameMenu:: No ExitButton in Widget Blueprint!", false);
    ExitButton->OnClicked.AddDynamic(this, &UMainGameMenu::ExitGame);

    return true;
}

void UMainGameMenu::StartGame()
{
    CHECK_NULLPTR_RET(MenuInterface, LogMenuWidget, "MainGameMenu:: No MenuInterface!");

    MenuInterface->StartGame();
}

void UMainGameMenu::ExitGame()
{
    CHECK_NULLPTR_RET(MenuInterface, LogMenuWidget, "MainGameMenu:: No MenuInterface!");

    bIsInteractive = false;
    MenuInterface->QuitToDesktop();
}

void UMainGameMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogMenuWidget, "Menu:: PlayerController is NULL!");

    bIsFocusable = true;
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UMainGameMenu::Teardown()
{
    Super::Teardown();
    
    if (bIsInteractive) {SetGameOnlyControls();}
}

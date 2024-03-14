// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "Components/Button.h"


UPopupMenu::UPopupMenu(const FObjectInitializer& ObjectInitializer) : UPopupWidget(ObjectInitializer)
{
	bIsFocusable = true;
}

bool UPopupMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(CloseButton, LogUserWidget, "PopupMenu:: No CloseButton in Widget Blueprint!", false);
    CloseButton->OnClicked.AddDynamic(this, &UPopupMenu::CloseMenu);

    return true;
}

void UPopupMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "PopupMenu:: PlayerController is NULL!");

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	InputModeData.SetHideCursorDuringCapture(false);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UPopupMenu::Teardown()
{
    if (bIsInteractive) {SetGameOnlyControls();}

    Super::Teardown();
}

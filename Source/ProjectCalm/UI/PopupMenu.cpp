// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "Components/Button.h"


bool UPopupMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(CloseButton, LogUserWidget, "PopupMenu:: No CloseButton in Widget Blueprint!", false);
    CloseButton->OnClicked.AddDynamic(this, &UPopupMenu::OnCloseButtonClicked);
    CloseButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);
    
    return true;
}

FReply UPopupMenu::NativeOnKeyDown(const FGeometry &InGeometry, const FKeyEvent &InKeyEvent)
{
    FReply Reply = Super::NativeOnKeyDown(InGeometry, InKeyEvent);
	if (InKeyEvent.GetKey() == EKeys::Escape)
    {
        Teardown();
        return FReply::Handled();
    }

    return Reply;
}

void UPopupMenu::OnCloseButtonClicked()
{
    PlayButtonPressedSound();
    Teardown();
}

void UPopupMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "PopupMenu:: PlayerController is NULL!");

    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UPopupMenu::Teardown()
{
    if (bIsInteractive) {SetGameOnlyControls();}

    Super::Teardown();
}

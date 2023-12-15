// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/LogMacros.h"

DEFINE_LOG_CATEGORY(LogMenuWidget)


UMenu::UMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    
}

bool UMenu::Initialize()
{
    return Super::Initialize();
}

void UMenu::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
    MenuInterface = NewMenuInterface;
}

void UMenu::Setup(bool bIsInteractiveIn)  // TODO: UPDATE FOR NEW INPUT SYSTEM
{
    bIsInteractive = bIsInteractiveIn;

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogMenuWidget, "Menu:: PlayerController is NULL!");
    
    AddToViewport();
    if (!bIsInteractive) {return;}
    
    bIsFocusable = true;
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UMenu::Teardown()
{
    RemoveFromParent();

    if (bIsInteractive) {SetGameOnlyControls();}
}

void UMenu::NativeDestruct()
{
    Teardown();

    Super::NativeDestruct();
}

void UMenu::SetGameOnlyControls()
{
    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogMenuWidget, "Menu:: PlayerController is NULL!");

    FInputModeGameOnly InputModeData;
    InputModeData.SetConsumeCaptureMouseDown(false);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}

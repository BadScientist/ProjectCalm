// Fill out your copyright notice in the Description page of Project Settings.


#include "Menu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/LogMacros.h"


UMenu::UMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
    bIsFocusable = true;
}

void UMenu::SetMenuInterface(IMenuInterface* NewMenuInterface)
{
    MenuInterface = NewMenuInterface;
}

void UMenu::Setup(bool bIsInteractiveIn)  // TODO: UPDATE FOR NEW INPUT SYSTEM
{
    bIsInteractive = bIsInteractiveIn;
    
    AddToViewport();
}

void UMenu::Teardown()
{
    RemoveFromParent();
}

void UMenu::NativeDestruct()
{
    Teardown();

    Super::NativeDestruct();
}

void UMenu::SetGameOnlyControls()
{
    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "Menu:: PlayerController is NULL!");

    FInputModeGameOnly InputModeData;
    InputModeData.SetConsumeCaptureMouseDown(false);
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = false;
}

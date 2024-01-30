// Fill out your copyright notice in the Description page of Project Settings.


#include "PopupMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "Components/Button.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


UPopupMenu::UPopupMenu(const FObjectInitializer& ObjectInitializer) : UMenu(ObjectInitializer)
{
	bIsFocusable = true;
}

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
    CHECK_NULLPTR_RET(MenuInterface, LogMenuWidget, "PopupMenu:: No MenuInterface!");

    MenuInterface->ClosePopupMenu(this);
}

void UPopupMenu::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogMenuWidget, "Menu:: PlayerController is NULL!");

    FInputModeGameAndUI InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	InputModeData.SetHideCursorDuringCapture(false);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;

	// if (MenuMappingContext != nullptr)
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PCPlayerStatics::GetEnhancedInputLocalPlayerSubsystem(this))
	// 	{
	// 		Subsystem->AddMappingContext(MenuMappingContext, 1);
	// 	}
	// }

	// if (CloseAction != nullptr)
	// {
	// 	if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
	// 	{		
	// 		ActionBinding = &(EnhancedInputComponent->BindAction(CloseAction, ETriggerEvent::Triggered, this, &UPopupMenu::CloseMenu));
	// 	}
	// }
}

void UPopupMenu::Teardown()
{    
    if (bIsInteractive) {SetGameOnlyControls();}
    
	// if (CloseAction != nullptr)
	// {
	// 	if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
	// 	{		
	// 		if (ActionBinding != nullptr)
    //         {
    //             EnhancedInputComponent->RemoveActionBindingForHandle(ActionBinding->GetHandle());
    //             ActionBinding = nullptr;
    //         }
	// 	}
	// }

	// if (MenuMappingContext != nullptr)
	// {
	// 	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PCPlayerStatics::GetEnhancedInputLocalPlayerSubsystem(this))
	// 	{
	// 		Subsystem->RemoveMappingContext(MenuMappingContext);
	// 	}
	// }

    Super::Teardown();
}

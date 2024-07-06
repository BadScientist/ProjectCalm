// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCalmGameInstance.h"
#include "ProjectCalm/UI/PauseMenu.h"
#include "ProjectCalm/UI/InventoryMenu.h"
#include "ProjectCalm/UI/QuestLog.h"
#include "ProjectCalm/UI/DialogueBox.h"
#include "ProjectCalm/UI/Dialogue.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#if WITH_EDITORONLY_DATA
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Inventory/InventoryComponent.h"
#endif

#include "Kismet/GameplayStatics.h"


UProjectCalmGameInstance::UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer)
{
    UE_LOG(LogLoad, Warning, TEXT("Creating ProjectCalmGameInstance..."));
}

void UProjectCalmGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Warning, TEXT("ProjectCalmGameInstance::Init()"));

}

void UProjectCalmGameInstance::StartGame()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "ProjectCalmGameInstance:: PlayerController is NULL!");

    GameplayMap.LoadSynchronous();
    CHECK_SOFTPTR_RET(GameplayMap, LogLoad, "ProjectCalmGameInstance:: GameplayMap is NULL!");
    UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController, GameplayMap);
}

void UProjectCalmGameInstance::QuitToMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "ProjectCalmGameInstance:: PlayerController is NULL!");

    MainMenuMap.LoadSynchronous();
    CHECK_SOFTPTR_RET(MainMenuMap, LogLoad, "ProjectCalmGameInstance:: MainMenuMap is NULL!");
    UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController, MainMenuMap);
}

void UProjectCalmGameInstance::QuitToDesktop()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "ProjectCalmGameInstance:: PlayerController is NULL!");
    PlayerController->ConsoleCommand(FString("quit"), true);
}

void UProjectCalmGameInstance::LoadMainMenu()
{
    CHECK_NULLPTR_RET(MainMenuClass, LogLoad, "ProjectCalmGameInstance:: MainMenu class is NULL!");
    UMenu* Menu = CreateWidget<UMenu>(this, MainMenuClass);    
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::LoadLoadingScreen()
{
    CHECK_NULLPTR_RET(LoadingScreenClass, LogLoad, "ProjectCalmGameInstance:: LoadingScreen class is NULL!");
    UMenu* Menu = CreateWidget<UMenu>(this, LoadingScreenClass);    
    SetupMenuWidget(Menu, false);
}

void UProjectCalmGameInstance::LoadPauseMenu()
{
    CHECK_NULLPTR_RET(PauseMenuClass, LogLoad, "ProjectCalmGameInstance:: PauseMenu class is NULL!");
    UPauseMenu* Menu = CreateWidget<UPauseMenu>(this, PauseMenuClass);
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::LoadInventoryMenu()
{
    CHECK_NULLPTR_RET(InventoryMenuClass, LogLoad, "ProjectCalmGameInstance:: InventoryMenu class is NULL!");
    UInventoryMenu* Menu = CreateWidget<UInventoryMenu>(this, InventoryMenuClass);
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::LoadQuestLog()
{
    CHECK_NULLPTR_RET(QuestLogClass, LogLoad, "ProjectCalmGameInstance:: InventoryMenu class is NULL!");
    UQuestLog* Menu = CreateWidget<UQuestLog>(this, QuestLogClass);
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::LoadDialogueBox(FDialogue Dialogue)
{
    CHECK_NULLPTR_RET(DialogueBoxClass, LogLoad, "ProjectCalmGameInstance:: DialogueBox class is NULL!");
    UDialogueBox* DialogueBox = CreateWidget<UDialogueBox>(this, DialogueBoxClass);
    SetupMenuWidget(DialogueBox, true);

    DialogueBox->SetDialogue(Dialogue);
}

void UProjectCalmGameInstance::SetupMenuWidget(UMenu* Menu, bool bIsInteractable)
{
    CHECK_NULLPTR_RET(Menu, LogUserWidget, "ProjectCalmGameInstance:: Failed to create menu widget!");
    Menu->SetMenuInterface(this);
    Menu->Setup(bIsInteractable);
}


// DEBUG COMMANDS

void UProjectCalmGameInstance::AddItem(int32 ItemID)
{
    if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this))
    {
        if (UInventoryComponent* InventoryComp = PlayerCharacter->GetInventoryComponent())
        {
            InventoryComp->AddItem(ItemID);
        }
    }
}

// END DEBUG COMMANDS

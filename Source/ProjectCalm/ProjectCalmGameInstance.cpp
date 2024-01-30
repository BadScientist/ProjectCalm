// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCalmGameInstance.h"
#include "UI/PauseMenu.h"
#include "UI/InventoryMenu.h"
#include "Utilities/LogMacros.h"

#if WITH_EDITORONLY_DATA
#include "Utilities/PCPlayerStatics.h"
#include "Inventory/InventoryComponent.h"
#endif

#include "Kismet/GameplayStatics.h"


UProjectCalmGameInstance::UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer)
{
    UE_LOG(LogLoad, Warning, TEXT("Creating ProjectCalmGameInstance..."));
    ConstructorHelpers::FClassFinder<UMenu> MainMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_MainMenu"));
    CHECK_NULLPTR_RET(MainMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find MainMenu blueprint class!");
    MainMenuClass = MainMenuBPClass.Class;
    
    ConstructorHelpers::FClassFinder<UMenu> LoadingScreenBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_LoadingScreen"));
    CHECK_NULLPTR_RET(LoadingScreenBPClass.Class, LogLoad, "PCGameInstance:: Could not find LoadingScreen blueprint class!");
    LoadingScreenClass = LoadingScreenBPClass.Class;

    ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_PauseMenu"));
    CHECK_NULLPTR_RET(PauseMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find PauseMenu blueprint class!");
    PauseMenuClass = PauseMenuBPClass.Class;    
    
    ConstructorHelpers::FClassFinder<UPopupMenu> InventoryMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_Inventory"));
    CHECK_NULLPTR_RET(InventoryMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find InventoryMenu blueprint class!");
    InventoryMenuClass = InventoryMenuBPClass.Class;
}

void UProjectCalmGameInstance::Init()
{
    Super::Init();

    UE_LOG(LogTemp, Warning, TEXT("PCGameInstance::Init()"));

}

void UProjectCalmGameInstance::StartGame()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "PCGameInstance:: PlayerController is NULL!");

    GameplayMap.LoadSynchronous();
    CHECK_SOFTPTR_RET(GameplayMap, LogLoad, "PCGameInstance:: GameplayMap is NULL!");
    UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController, GameplayMap);
}

void UProjectCalmGameInstance::QuitToMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "PCGameInstance:: PlayerController is NULL!");

    MainMenuMap.LoadSynchronous();
    CHECK_SOFTPTR_RET(MainMenuMap, LogLoad, "PCGameInstance:: MainMenuMap is NULL!");
    UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController, MainMenuMap);
}

void UProjectCalmGameInstance::QuitToDesktop()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "PCGameInstance:: PlayerController is NULL!");
    PlayerController->ConsoleCommand(FString("quit"), true);
}

void UProjectCalmGameInstance::ClosePopupMenu(UPopupMenu* PopupMenu)
{
    if (MenuStack.Remove(PopupMenu)) {PopupMenu->Teardown();}
}

void UProjectCalmGameInstance::ClosePopupMenu()
{
    UPopupMenu* Menu = MenuStack.Pop();
    if (Menu != nullptr) {Menu->Teardown();}
}

bool UProjectCalmGameInstance::IsPopupMenuOpen(UPopupMenu* PopupMenu)
{
    return MenuStack.Contains(PopupMenu);
}

bool UProjectCalmGameInstance::IsPopupMenuOpen()
{
    return !MenuStack.IsEmpty();
}

void UProjectCalmGameInstance::LoadMainMenu()
{
    CHECK_NULLPTR_RET(MainMenuClass, LogLoad, "PCGameInstance:: MainMenu class is NULL!");
    UMenu* Menu = CreateWidget<UMenu>(this, MainMenuClass);    
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::LoadLoadingScreen()
{
    CHECK_NULLPTR_RET(LoadingScreenClass, LogLoad, "PCGameInstance:: LoadingScreen class is NULL!");
    UMenu* Menu = CreateWidget<UMenu>(this, LoadingScreenClass);    
    SetupMenuWidget(Menu, false);
}

void UProjectCalmGameInstance::LoadPauseMenu()
{
    CHECK_NULLPTR_RET(PauseMenuClass, LogLoad, "PCGameInstance:: PauseMenu class is NULL!");
    UPauseMenu* Menu = CreateWidget<UPauseMenu>(this, PauseMenuClass);    
    SetupMenuWidget(Menu, true);

    MenuStack.Push(Menu);
}

void UProjectCalmGameInstance::LoadInventoryMenu()
{
    CHECK_NULLPTR_RET(InventoryMenuClass, LogLoad, "PCGameInstance:: InventoryMenu class is NULL!");
    UInventoryMenu* Menu = CreateWidget<UInventoryMenu>(this, InventoryMenuClass);
    SetupMenuWidget(Menu, true);
    
    MenuStack.Push(Menu);
}

void UProjectCalmGameInstance::SetupMenuWidget(UMenu* Menu, bool bIsInteractable)
{
    CHECK_NULLPTR_RET(Menu, LogMenuWidget, "PCGameInstance:: Failed to create menu widget!");
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

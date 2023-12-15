// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCalmGameInstance.h"
#include "UI/PauseMenu.h"
#include "Utilities/LogMacros.h"


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

    PlayerController->ClientTravel("/Game/ProjectCalm/Maps/GameplayMap", ETravelType::TRAVEL_Absolute);
}

void UProjectCalmGameInstance::QuitToMainMenu()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "PCGameInstance:: PlayerController is NULL!");

    PlayerController->ClientTravel("/Game/ProjectCalm/Maps/MainMenuMap", ETravelType::TRAVEL_Absolute);
}

void UProjectCalmGameInstance::QuitToDesktop()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "PCGameInstance:: PlayerController is NULL!");
    PlayerController->ConsoleCommand(FString("quit"), true);
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
}

void UProjectCalmGameInstance::LoadInventoryMenu()
{
    CHECK_NULLPTR_RET(InventoryMenuClass, LogLoad, "PCGameInstance:: InventoryMenu class is NULL!");
    UPopupMenu* Menu = CreateWidget<UPopupMenu>(this, InventoryMenuClass);
    SetupMenuWidget(Menu, true);
}

void UProjectCalmGameInstance::SetupMenuWidget(UMenu* Menu, bool bIsInteractable)
{
    CHECK_NULLPTR_RET(Menu, LogMenuWidget, "PCGameInstance:: Failed to create menu widget!");
    Menu->SetMenuInterface(this);
    Menu->Setup(bIsInteractable);
}

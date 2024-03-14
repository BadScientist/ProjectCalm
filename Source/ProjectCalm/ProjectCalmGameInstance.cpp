// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCalmGameInstance.h"
#include "UI/PauseMenu.h"
#include "UI/InventoryMenu.h"
#include "UI/FancyTextDisplay.h"
#include "Utilities/LogMacros.h"

#if WITH_EDITORONLY_DATA
#include "Utilities/PCPlayerStatics.h"
#include "Inventory/InventoryComponent.h"
#endif

#include "Kismet/GameplayStatics.h"


UProjectCalmGameInstance::UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer)
{
    UE_LOG(LogLoad, Warning, TEXT("Creating ProjectCalmGameInstance..."));
    // ConstructorHelpers::FClassFinder<UMenu> MainMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_MainMenu"));
    // CHECK_NULLPTR_RET(MainMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find MainMenu blueprint class!");
    // MainMenuClass = MainMenuBPClass.Class;
    
    // ConstructorHelpers::FClassFinder<UMenu> LoadingScreenBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_LoadingScreen"));
    // CHECK_NULLPTR_RET(LoadingScreenBPClass.Class, LogLoad, "PCGameInstance:: Could not find LoadingScreen blueprint class!");
    // LoadingScreenClass = LoadingScreenBPClass.Class;

    // ConstructorHelpers::FClassFinder<UPauseMenu> PauseMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_PauseMenu"));
    // CHECK_NULLPTR_RET(PauseMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find PauseMenu blueprint class!");
    // PauseMenuClass = PauseMenuBPClass.Class;    
    
    // ConstructorHelpers::FClassFinder<UPopupMenu> InventoryMenuBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_Inventory"));
    // CHECK_NULLPTR_RET(InventoryMenuBPClass.Class, LogLoad, "PCGameInstance:: Could not find InventoryMenu blueprint class!");
    // InventoryMenuClass = InventoryMenuBPClass.Class;
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

void UProjectCalmGameInstance::ClosePopup(UPopupWidget* Popup)
{
    if (Popup != nullptr && PopupStack.Remove(Popup)) {Popup->Teardown();}
}

void UProjectCalmGameInstance::ClosePopupMenu()
{
    if (UPopupMenu* TopPopupMenu = GetTopPopupMenu()) {ClosePopup(TopPopupMenu);} 
}

bool UProjectCalmGameInstance::IsPopupMenuOpen(UPopupMenu* PopupMenu)
{
    return PopupStack.Contains(PopupMenu);
}

bool UProjectCalmGameInstance::IsPopupMenuOpen()
{
    return GetTopPopupMenu() != nullptr;
}

bool UProjectCalmGameInstance::IsPopupOpen()
{
    return !PopupStack.IsEmpty();
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

    PopupStack.Push(Menu);
}

void UProjectCalmGameInstance::LoadInventoryMenu()
{
    CHECK_NULLPTR_RET(InventoryMenuClass, LogLoad, "PCGameInstance:: InventoryMenu class is NULL!");
    UInventoryMenu* Menu = CreateWidget<UInventoryMenu>(this, InventoryMenuClass);
    SetupMenuWidget(Menu, true);
    
    PopupStack.Push(Menu);
}

void UProjectCalmGameInstance::LoadDialogueBox()
{
    CHECK_NULLPTR_RET(DialogueBoxClass, LogLoad, "PCGameInstance:: DialogueBox class is NULL!");
    UFancyTextDisplay* DialogueBox = CreateWidget<UFancyTextDisplay>(this, DialogueBoxClass);
    SetupMenuWidget(DialogueBox, true);

    // TArray<FString> TestStrings{
    //     FString("This is a test string."),
    //     FString("This is a second test string for testing the dialogue box with a longer string of text."),
    //     FString("This is a third test string. \nThis is the same string after a newline character."),
    //     FString(("This is yet another test string to test the dialogue box with an even longer string of "
    //     "text that will not fit in a single dialogue box due to the character and line limits. This means "
    //     "that multiple pages of dialogue will need to be displayed sequentially in order to display all of "
    //     "the text in this one string. If all goes well, it will be divided up into parts and each part will "
    //     "be printed when the player presses the 'E' key.")),
    //     FString(("This is a string with multiple styles of text marked by tags:\nThis is "
    //     "{header}header{/header} text,  this is {spooky}spoooOOOoooky{/spooky} text and this is "
    //     "{scared}scared{/scared} text. The rest is default.\nLorem ipsum dolor sit amet, consectetur "
    //     "adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad "
    //     "minim veniam, quis nostrud exercitation ullamco laboris nisi ut {header}aliquip ex ea commodo "
    //     "consequat. Duis aute irure dolor in{/header} reprehenderit in voluptate velit {spooky}esse cillum "
    //     "dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui "
    //     "officia deserunt mollit anim id{/spooky} est laborum."))};
    // DialogueBox->SetStringsToDisplay(TestStrings);
    
    PopupStack.Push(DialogueBox);
}

void UProjectCalmGameInstance::SetupMenuWidget(UMenu* Menu, bool bIsInteractable)
{
    CHECK_NULLPTR_RET(Menu, LogUserWidget, "PCGameInstance:: Failed to create menu widget!");
    Menu->SetMenuInterface(this);
    Menu->Setup(bIsInteractable);
}

UPopupMenu *UProjectCalmGameInstance::GetTopPopupMenu()
{
    UPopupWidget* Popup{nullptr};
    UPopupMenu* PopupMenu{nullptr};
    int32 Idx = 0;
    do
    {
        Popup = PopupStack.Peak(Idx);
        PopupMenu = Cast<UPopupMenu>(Popup);
        if (PopupMenu != nullptr) {return PopupMenu;}
        Idx++;
    } while (Popup != nullptr);

    return nullptr;
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

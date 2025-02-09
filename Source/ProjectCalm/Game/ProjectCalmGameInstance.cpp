// Fill out your copyright notice in the Description page of Project Settings.


#include "ProjectCalmGameInstance.h"
#include "SoundManager.h"
#include "ProjectCalm/UI/MainGameMenu.h"
#include "ProjectCalm/UI/PauseMenu.h"
#include "ProjectCalm/UI/InventoryMenu.h"
#include "ProjectCalm/UI/QuestLog.h"
#include "ProjectCalm/UI/PhotoLog.h"
#include "ProjectCalm/UI/DialogueBox.h"
#include "ProjectCalm/UI/Dialogue.h"
#include "ProjectCalm/UI/DeathScreen.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Photos/PhotoData.h"

#if WITH_EDITORONLY_DATA
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Inventory/InventoryComponent.h"
#endif

#include "Kismet/GameplayStatics.h"


UProjectCalmGameInstance::UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer)
{
    SoundManager = CreateDefaultSubobject<USoundManager>(TEXT("SoundManager"));
}

void UProjectCalmGameInstance::Init()
{
    Super::Init();

}

void UProjectCalmGameInstance::StartGame()
{
    LoadLoadingScreen();
    PendingMap = GameplayMap;
}

void UProjectCalmGameInstance::QuitToMainMenu()
{
    LoadLoadingScreen();
    PendingMap = MainMenuMap;
}

void UProjectCalmGameInstance::LoadPendingMap()
{
    APlayerController* PlayerController = GetFirstLocalPlayerController();
    CHECK_NULLPTR_RET(PlayerController, LogPlayerController, "ProjectCalmGameInstance:: PlayerController is NULL!");
    
    PendingMap.LoadSynchronous();
    CHECK_SOFTPTR_RET(PendingMap, LogLoad, "ProjectCalmGameInstance:: PendingMap is NULL!");
    UGameplayStatics::OpenLevelBySoftObjectPtr(PlayerController, PendingMap);
    PendingMap.Reset();
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

void UProjectCalmGameInstance::LoadPhotoLog(uint32 CameraID)
{
    CHECK_NULLPTR_RET(PhotoLogClass, LogLoad, "ProjectCalmGameInstance:: InventoryMenu class is NULL!");
    UPhotoLog* Menu = CreateWidget<UPhotoLog>(this, PhotoLogClass);
    SetupMenuWidget(Menu, true);
    Menu->PopulateLog(CameraID);
}

void UProjectCalmGameInstance::LoadDialogueBox(FDialogue Dialogue)
{
    CHECK_NULLPTR_RET(DialogueBoxClass, LogLoad, "ProjectCalmGameInstance:: DialogueBox class is NULL!");
    UDialogueBox* DialogueBox = CreateWidget<UDialogueBox>(this, DialogueBoxClass);
    SetupMenuWidget(DialogueBox, true);

    DialogueBox->SetDialogue(Dialogue);
}

void UProjectCalmGameInstance::LoadDeathScreen(FString DamageMessage)
{
    CHECK_NULLPTR_RET(DeathScreenClass, LogLoad, "ProjectCalmGameInstance:: DeathScreen class is NULL!");
    UDeathScreen* Menu = CreateWidget<UDeathScreen>(this, DeathScreenClass);
    SetupMenuWidget(Menu, true);

    Menu->SetCauseOfDeathText(DamageMessage);
}

void UProjectCalmGameInstance::PlayUISound(FName SoundName, UObject* WorldContextObject, bool bPersistOnLevelLoad)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->PlayUISound(SoundName, WorldContextObject, bPersistOnLevelLoad);
}

float UProjectCalmGameInstance::GetMasterVolume()
{
    CHECK_NULLPTR_RETVAL(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!", 1.0f);
    return SoundManager->GetMasterVolume();
}

float UProjectCalmGameInstance::GetMusicVolume()
{
    CHECK_NULLPTR_RETVAL(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!", 1.0f);
    return SoundManager->GetMusicVolume();
}

float UProjectCalmGameInstance::GetAmbientVolume()
{
    CHECK_NULLPTR_RETVAL(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!", 1.0f);
    return SoundManager->GetAmbientVolume();
}

float UProjectCalmGameInstance::GetSFXVolume()
{
    CHECK_NULLPTR_RETVAL(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!", 1.0f);
    return SoundManager->GetSFXVolume();
}

float UProjectCalmGameInstance::GetUIVolume()
{
    CHECK_NULLPTR_RETVAL(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!", 1.0f);
    return SoundManager->GetUIVolume();
}

void UProjectCalmGameInstance::SetMasterVolume(float InVolume)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->SetMasterVolume(InVolume);
}

void UProjectCalmGameInstance::SetMusicVolume(float InVolume)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->SetMusicVolume(InVolume);
}

void UProjectCalmGameInstance::SetAmbientVolume(float InVolume)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->SetAmbientVolume(InVolume);
}

void UProjectCalmGameInstance::SetSFXVolume(float InVolume)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->SetSFXVolume(InVolume);
}

void UProjectCalmGameInstance::SetUIVolume(float InVolume)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->SetUIVolume(InVolume);
}

void UProjectCalmGameInstance::PlayMusicOrAmbientSound(FName SoundName, UObject* WorldContextObject, bool bIsMusic, bool bPersistOnLevelLoad)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->PlayMusicOrAmbientSound(SoundName, WorldContextObject, bIsMusic, bPersistOnLevelLoad);
}

void UProjectCalmGameInstance::StopMusic(FName SoundName, float FadeDuration)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->StopMusic(SoundName, FadeDuration);
}

void UProjectCalmGameInstance::PlayDiageticSound(FName SoundName, UObject *WorldContextObject, FVector SourceLocation, float VolumeMultiplier)
{
    CHECK_NULLPTR_RET(SoundManager, LogAudio, "ProjectCalmGameInstance:: No SoundManager!");
    SoundManager->PlayDiageticSound(SoundName, WorldContextObject, SourceLocation, VolumeMultiplier);
}

void UProjectCalmGameInstance::SetupMenuWidget(UMenu* Menu, bool bIsInteractable)
{
    CHECK_NULLPTR_RET(Menu, LogUserWidget, "ProjectCalmGameInstance:: Failed to create menu widget!");
    Menu->SetMenuInterface(this);
    Menu->Setup(bIsInteractable);
}


// DEBUG COMMANDS

#if WITH_EDITORONLY_DATA

void UProjectCalmGameInstance::AddItem(int32 ItemID)
{
    if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this))
    {
        if (UInventoryComponent* InventoryComp = PlayerCharacter->GetInventoryComponent()) {InventoryComp->AddItem(ItemID);}
    }
}

#else

void UProjectCalmGameInstance::AddItem(int32 ItemID){}

#endif

// END DEBUG COMMANDS

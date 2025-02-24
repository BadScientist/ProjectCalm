// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ProjectCalm/UI/MenuInterface.h"
#include "ProjectCalmGameInstance.generated.h"

class UMenu;
class UMainGameMenu;
class UPopupMenu;
class UPauseMenu;
class UInventoryMenu;
class UQuestLog;
class UPhotoLog;
class UDialogueBox;
class UDeathScreen;
class USoundManager;
struct FDialogue;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UProjectCalmGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, Category = Maps)
	TSoftObjectPtr<UWorld> MainMenuMap;
	UPROPERTY(EditAnywhere, Category = Maps)
	TSoftObjectPtr<UWorld> GameplayMap;

    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UPauseMenu> PauseMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UInventoryMenu> InventoryMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UQuestLog> QuestLogClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UPhotoLog> PhotoLogClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UMainGameMenu> MainMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UMenu> LoadingScreenClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UDialogueBox> DialogueBoxClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UDeathScreen> DeathScreenClass;

    UPROPERTY(EditDefaultsOnly, Category = SoundManager)
	USoundManager* SoundManager;
	
	TSoftObjectPtr<UWorld> PendingMap;

public:
	UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init() override;

	// Start Menu Interface Implementation
	UFUNCTION(exec)
	virtual void StartGame() override;
	UFUNCTION(exec)
	virtual void QuitToMainMenu() override;
	UFUNCTION(BlueprintCallable)
	void LoadPendingMap();
	UFUNCTION(exec)
	virtual void QuitToDesktop() override;
	UFUNCTION(BlueprintCallable)
	virtual void PlayUISound(FName SoundName, UObject* WorldContextObject, bool bPersistOnLevelLoad) override;
	virtual float GetMasterVolume() override;
	virtual float GetMusicVolume() override;
	virtual float GetAmbientVolume() override;
	virtual float GetSFXVolume() override;
	virtual float GetUIVolume() override;
	virtual void SetMasterVolume(float InVolume) override;
	virtual void SetMusicVolume(float InVolume) override;
	virtual void SetAmbientVolume(float InVolume) override;
	virtual void SetSFXVolume(float InVolume) override;
	virtual void SetUIVolume(float InVolume) override;
	// End Menu Interface Implementation

	UFUNCTION(BlueprintCallable)
	virtual void PlayMusicOrAmbientSound(FName SoundName, UObject* WorldContextObject, bool bIsMusic, bool bPersistOnLevelLoad);
	UFUNCTION(BlueprintCallable)
	virtual void StopMusic(FName SoundName, float FadeDuration = -1.0f);
    virtual void PlayDiageticSound(FName SoundName, UObject* WorldContextObject, FVector SourceLocation, float VolumeMultiplier = 1.0f);
	
	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();
	UFUNCTION(BlueprintCallable)
	void LoadLoadingScreen();
	UFUNCTION()
	void LoadPauseMenu();
	UFUNCTION()
	void LoadInventoryMenu();
	UFUNCTION()
	void LoadQuestLog();
	UFUNCTION()
	void LoadPhotoLog(uint32 CameraID);
	UFUNCTION()
	void LoadDialogueBox(FDialogue Dialogue);
	UFUNCTION()
	void LoadDeathScreen(FString DamageMessage);

	// DEBUG COMMANDS

	UFUNCTION(exec)
	void AddItem(int32 ItemID);
	
	//END DEBUG COMMANDS

private:
	void SetupMenuWidget(UMenu* Menu, bool bIsInteractable);
	
};

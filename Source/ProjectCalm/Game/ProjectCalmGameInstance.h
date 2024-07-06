// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "ProjectCalm/UI/MenuInterface.h"
#include "ProjectCalmGameInstance.generated.h"

class UMenu;
class UPopupMenu;
class UPauseMenu;
class UInventoryMenu;
class UQuestLog;
class UDialogueBox;
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
	TSubclassOf<UMenu> MainMenuClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UMenu> LoadingScreenClass;
    UPROPERTY(EditDefaultsOnly, Category = WidgetClasses)
	TSubclassOf<UDialogueBox> DialogueBoxClass;


public:
	UProjectCalmGameInstance(const FObjectInitializer &ObjectInitializer);
	virtual void Init() override;

	// Start Menu Interface Implementation
	UFUNCTION(exec)
	virtual void StartGame() override;
	UFUNCTION(exec)
	virtual void QuitToMainMenu() override;
	UFUNCTION(exec)
	virtual void QuitToDesktop() override;
	// End Menu Interface Implementation
	
	UFUNCTION(exec, BlueprintCallable)
	void LoadMainMenu();
	UFUNCTION(exec, BlueprintCallable)
	void LoadLoadingScreen();
	UFUNCTION(exec, BlueprintCallable)
	void LoadPauseMenu();
	UFUNCTION(exec, BlueprintCallable)
	void LoadInventoryMenu();
	UFUNCTION(exec, BlueprintCallable)
	void LoadQuestLog();
	UFUNCTION(BlueprintCallable)
	void LoadDialogueBox(FDialogue Dialogue);


	// DEBUG COMMANDS

	UFUNCTION(exec)
	void AddItem(int32 ItemID);
	
	//END DEBUG COMMANDS

private:
	void SetupMenuWidget(UMenu* Menu, bool bIsInteractable);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "UI/MenuInterface.h"
#include "UI/PopupMenuStack.h"
#include "ProjectCalmGameInstance.generated.h"

class UMenu;
class UPopupMenu;
class UPauseMenu;
class UInventoryMenu;


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

	TSubclassOf<UPauseMenu> PauseMenuClass;
	TSubclassOf<UInventoryMenu> InventoryMenuClass;
	TSubclassOf<UMenu> MainMenuClass;
	TSubclassOf<UMenu> LoadingScreenClass;
	// class UMainMenu* MainMenu;

	FPopupMenuStack MenuStack{FPopupMenuStack()};


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
	virtual void ClosePopupMenu(UPopupMenu* PopupMenu) override;
	// End Menu Interface Implementation

	UFUNCTION(BlueprintCallable)
	void ClosePopupMenu();
	bool IsPopupMenuOpen(UPopupMenu* PopupMenu);
	bool IsPopupMenuOpen();
	
	UFUNCTION(exec, BlueprintCallable)
	void LoadMainMenu();
	UFUNCTION(exec, BlueprintCallable)
	void LoadLoadingScreen();
	UFUNCTION(exec, BlueprintCallable)
	void LoadPauseMenu();
	UFUNCTION(exec, BlueprintCallable)
	void LoadInventoryMenu();


	// DEBUG COMMANDS

	UFUNCTION(exec)
	void AddItem(int32 ItemID);
	
	//END DEBUG COMMANDS

private:
	void SetupMenuWidget(UMenu* Menu, bool bIsInteractable);
	
};

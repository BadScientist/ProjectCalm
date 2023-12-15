// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"

#include "UI/MenuInterface.h"
#include "ProjectCalmGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UProjectCalmGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()

	TSubclassOf<class UPauseMenu> PauseMenuClass;
	TSubclassOf<class UPopupMenu> InventoryMenuClass;
	TSubclassOf<class UMenu> MainMenuClass;
	TSubclassOf<class UMenu> LoadingScreenClass;
	// class UMainMenu* MainMenu;


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

private:
	void SetupMenuWidget(UMenu* Menu, bool bIsInteractable);
	
};

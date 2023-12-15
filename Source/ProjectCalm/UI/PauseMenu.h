// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "PauseMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPauseMenu : public UPopupMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
    
	UFUNCTION()
	void QuitToMainMenu();
	
};

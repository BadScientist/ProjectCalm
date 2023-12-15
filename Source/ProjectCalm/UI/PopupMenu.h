// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "PopupMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPopupMenu : public UMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UFUNCTION()
	void CloseMenu();
	
};

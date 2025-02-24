// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "PauseMenu.generated.h"

class UWidgetSwitcher;
class UOptionsMenu;

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
	UButton* ResumeButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	UOptionsMenu* OptionsMenu;
    
	UFUNCTION()
	void QuitToMainMenu();
	UFUNCTION()
	void SwitchToOptionsMenu();

protected:
	virtual void OnCloseButtonClicked() override;

public:
	virtual void SetMenuInterface(IMenuInterface* NewMenuInterface) override;
	virtual void Setup(bool bIsInteractiveIn) override;
	virtual void Teardown() override;
};

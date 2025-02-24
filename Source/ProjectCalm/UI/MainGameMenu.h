// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "MainGameMenu.generated.h"

class UWidgetSwitcher;
class UOptionsMenu;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UMainGameMenu : public UMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	UWidgetSwitcher* MenuSwitcher;
	UPROPERTY(meta = (BindWidget))
	UOptionsMenu* OptionsMenu;

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void SwitchToOptionsMenu();
	UFUNCTION()
	void ExitGame();

public:
	virtual void SetMenuInterface(IMenuInterface* NewMenuInterface) override;
	virtual void Setup(bool bIsInteractiveIn) override;
	virtual void Teardown() override;

private:
	UFUNCTION(BlueprintCallable)
	void PlayStartButtonPressedSound();
	
};

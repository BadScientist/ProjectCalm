// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "OptionsMenu.generated.h"

class USlider;
class UComboBoxString;
class UWidgetSwitcher;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UOptionsMenu : public UMenu
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UButton* BackButton;

	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSliderMaster;
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSliderMusic;
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSliderAmbient;
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSliderSFX;
	UPROPERTY(meta = (BindWidget))
	USlider* VolumeSliderUI;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* WindowModeDropdown;
	UPROPERTY(meta = (BindWidget))
	UComboBoxString* ResolutionDropdown;

	UWidgetSwitcher* WidgetSwitcher;

protected:
	virtual bool Initialize() override;

public:
	virtual void Setup(bool bIsInteractiveIn) override;
	void SetWidgetSwitcher(UWidgetSwitcher* InWidgetSwitcher) {WidgetSwitcher = InWidgetSwitcher;};

	UFUNCTION()
	void Back();

private:
	UFUNCTION()
	void OnMasterVolumeChanged(float NewValue);
	UFUNCTION()
	void OnMusicVolumeChanged(float NewValue);
	UFUNCTION()
	void OnAmbientVolumeChanged(float NewValue);
	UFUNCTION()
	void OnSFXVolumeChanged(float NewValue);
	UFUNCTION()
	void OnUIVolumeChanged(float NewValue);
	UFUNCTION()
	void OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	UFUNCTION()
	void OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType);
	
};

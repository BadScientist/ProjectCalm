// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "OptionsMenu.generated.h"

class USlider;
class UComboBoxString;
class UWidgetSwitcher;
class UVerticalBox;
class UTextBlock;

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
	UButton* ConfirmButton;
	UPROPERTY(meta = (BindWidget))
	UButton* CancelButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TimerText;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OptionsContainer;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* ConfirmationBox;

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

	bool bAwaitingConfirmation{false};
	FTimerHandle ConfirmationTimerHandle;
	FString PreviousSetting;
	float CurrentTime{0.0f};
	float ConfirmationStartTime{0.0f};

protected:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void Setup(bool bIsInteractiveIn) override;
	void SetWidgetSwitcher(UWidgetSwitcher* InWidgetSwitcher) {WidgetSwitcher = InWidgetSwitcher;};
	bool IsAwaitingConfirmation() {return bAwaitingConfirmation;}

	UFUNCTION()
	void Back();
	UFUNCTION()
	void Confirm();
	UFUNCTION()
	void Cancel();

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
	UFUNCTION()
	void OnResolutionOpened();

	void StartConfirmation();
	void EndConfirmation(bool bConfirmed);
	
};

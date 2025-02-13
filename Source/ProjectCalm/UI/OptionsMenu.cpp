// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenu.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"
#include "Components/Slider.h"
#include "Components/ComboBoxString.h"
#include "Components/WidgetSwitcher.h"
#include "Components/VerticalBox.h"
#include "Components/TextBlock.h"
#include "GameFramework/GameUserSettings.h"

#ifdef PC_DEBUG_LOGS
    #define LOCAL_DEBUG_LOGS
#endif // DEBUG


bool UOptionsMenu::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(BackButton, LogUserWidget, "OptionsMenu:: No BackButton in Widget Blueprint!", false);
    BackButton->OnClicked.AddDynamic(this, &UOptionsMenu::Back);
    BackButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(ConfirmButton, LogUserWidget, "OptionsMenu:: No ConfirmButton in Widget Blueprint!", false);
    ConfirmButton->OnClicked.AddDynamic(this, &UOptionsMenu::Confirm);
    ConfirmButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(CancelButton, LogUserWidget, "OptionsMenu:: No CancelButton in Widget Blueprint!", false);
    CancelButton->OnClicked.AddDynamic(this, &UOptionsMenu::Cancel);
    CancelButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(VolumeSliderMaster, LogUserWidget, "OptionsMenu:: No VolumeSliderMaster in Widget Blueprint!", false);
    VolumeSliderMaster->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnMasterVolumeChanged);

    CHECK_NULLPTR_RETVAL(VolumeSliderMusic, LogUserWidget, "OptionsMenu:: No VolumeSliderMusic in Widget Blueprint!", false);
    VolumeSliderMusic->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnMusicVolumeChanged);

    CHECK_NULLPTR_RETVAL(VolumeSliderAmbient, LogUserWidget, "OptionsMenu:: No VolumeSliderAmbient in Widget Blueprint!", false);
    VolumeSliderAmbient->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnAmbientVolumeChanged);

    CHECK_NULLPTR_RETVAL(VolumeSliderSFX, LogUserWidget, "OptionsMenu:: No VolumeSliderSFX in Widget Blueprint!", false);
    VolumeSliderSFX->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnSFXVolumeChanged);

    CHECK_NULLPTR_RETVAL(VolumeSliderUI, LogUserWidget, "OptionsMenu:: No VolumeSliderUI in Widget Blueprint!", false);
    VolumeSliderUI->OnValueChanged.AddDynamic(this, &UOptionsMenu::OnUIVolumeChanged);

    CHECK_NULLPTR_RETVAL(WindowModeDropdown, LogUserWidget, "OptionsMenu:: No WindowModeDropdown in Widget Blueprint!", false);
    WindowModeDropdown->OnSelectionChanged.AddDynamic(this, &UOptionsMenu::OnWindowModeChanged);

    CHECK_NULLPTR_RETVAL(ResolutionDropdown, LogUserWidget, "OptionsMenu:: No ResolutionDropdown in Widget Blueprint!", false);
    ResolutionDropdown->OnSelectionChanged.AddDynamic(this, &UOptionsMenu::OnResolutionChanged);    
    ResolutionDropdown->OnOpening.AddDynamic(this, &UOptionsMenu::OnResolutionOpened);

    return true;
}

void UOptionsMenu::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    CurrentTime += InDeltaTime;

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogUserWidget, Display, TEXT("OptionsMenu:: Tick: bAwaitingConfirmation: %i"), bAwaitingConfirmation);
#endif // DEBUG

    if (!bAwaitingConfirmation) {return;}

    float TimeRemaining = 15.0f - (CurrentTime - ConfirmationStartTime);

    if (TimerText != nullptr) {TimerText->SetText(FText::FromString(FString::FromInt(FMath::CeilToInt32(TimeRemaining))));}

    if (TimeRemaining <= 0.0f) {EndConfirmation(false);}
}

void UOptionsMenu::Setup(bool bIsInteractiveIn)
{
    bIsInteractive = bIsInteractiveIn;

    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "OptionsMenu:: No MenuInterface set!");

    CHECK_NULLPTR_RET(VolumeSliderMaster, LogUserWidget, "OptionsMenu:: No VolumeSliderMaster in Widget Blueprint!");
    VolumeSliderMaster->SetValue(100 * MenuInterface->GetMasterVolume());

    CHECK_NULLPTR_RET(VolumeSliderMusic, LogUserWidget, "OptionsMenu:: No VolumeSliderMusic in Widget Blueprint!");
    VolumeSliderMusic->SetValue(100 * MenuInterface->GetMusicVolume());

    CHECK_NULLPTR_RET(VolumeSliderAmbient, LogUserWidget, "OptionsMenu:: No VolumeSliderAmbient in Widget Blueprint!");
    VolumeSliderAmbient->SetValue(100 * MenuInterface->GetAmbientVolume());

    CHECK_NULLPTR_RET(VolumeSliderSFX, LogUserWidget, "OptionsMenu:: No VolumeSliderSFX in Widget Blueprint!");
    VolumeSliderSFX->SetValue(100 * MenuInterface->GetSFXVolume());

    CHECK_NULLPTR_RET(VolumeSliderUI, LogUserWidget, "OptionsMenu:: No VolumeSliderUI in Widget Blueprint!");
    VolumeSliderUI->SetValue(100 * MenuInterface->GetUIVolume());

    CHECK_NULLPTR_RET(GEngine, LogUserWidget, "OptionsMenu:: GEngine is NULL!");
    
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    CHECK_NULLPTR_RET(Settings, LogUserWidget, "OptionsMenu:: Could not retrieve Game User Settings!");

    EWindowMode::Type WindowMode = Settings->GetFullscreenMode();
    CHECK_NULLPTR_RET(WindowModeDropdown, LogUserWidget, "OptionsMenu:: No WindowModeDropdown in Widget Blueprint!");
    WindowModeDropdown->SetSelectedOption(WindowMode == EWindowMode::Fullscreen ? FString("Fullscreen") : WindowMode == EWindowMode::Windowed ? FString("Windowed") : FString("Borderless"));

    FIntPoint ScreenWidth = Settings->GetScreenResolution();
    CHECK_NULLPTR_RET(ResolutionDropdown, LogUserWidget, "OptionsMenu:: No ResolutionDropdown in Widget Blueprint!");    
    FString SelectedOption(FString::FromInt(ScreenWidth.X) + " x " + FString::FromInt(ScreenWidth.Y));
    ResolutionDropdown->SetSelectedOption(SelectedOption);
}

void UOptionsMenu::Back()
{
    PlayButtonPressedSound();

    CHECK_NULLPTR_RET(WidgetSwitcher, LogUserWidget, "UOptionsMenu:: No WidgetSwitcher set!");
    const int32 Idx = WidgetSwitcher->GetActiveWidgetIndex();
    WidgetSwitcher->SetActiveWidgetIndex(Idx > 0 ? Idx - 1 : 0);
}

void UOptionsMenu::Confirm()
{
    EndConfirmation(true);
}

void UOptionsMenu::Cancel()
{
    EndConfirmation(false);
}

void UOptionsMenu::OnMasterVolumeChanged(float NewValue)
{
    float Volume = FMath::Clamp(NewValue / 100, 0, 1);
    MenuInterface->SetMasterVolume(Volume);
}

void UOptionsMenu::OnMusicVolumeChanged(float NewValue)
{
    float Volume = FMath::Clamp(NewValue / 100, 0, 1);
    MenuInterface->SetMusicVolume(Volume);
}

void UOptionsMenu::OnAmbientVolumeChanged(float NewValue)
{
    float Volume = FMath::Clamp(NewValue / 100, 0, 1);
    MenuInterface->SetAmbientVolume(Volume);
}

void UOptionsMenu::OnSFXVolumeChanged(float NewValue)
{
    float Volume = FMath::Clamp(NewValue / 100, 0, 1);
    MenuInterface->SetSFXVolume(Volume);
}

void UOptionsMenu::OnUIVolumeChanged(float NewValue)
{
    float Volume = FMath::Clamp(NewValue / 100, 0, 1);
    MenuInterface->SetUIVolume(Volume);
}

void UOptionsMenu::OnWindowModeChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct) {return;}
    CHECK_NULLPTR_RET(GEngine, LogUserWidget, "OptionsMenu:: GEngine is NULL!");
    
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    CHECK_NULLPTR_RET(Settings, LogUserWidget, "OptionsMenu:: Could not retrieve Game User Settings!");

    EWindowMode::Type WindowType;
    if (SelectedItem == FString("Fullscreen")) {WindowType = EWindowMode::Fullscreen;}
    else if (SelectedItem == FString("Windowed")) {WindowType = EWindowMode::Windowed;}
    else {WindowType = EWindowMode::WindowedFullscreen;}
    Settings->SetFullscreenMode(WindowType);
    Settings->ApplySettings(false);
}

void UOptionsMenu::OnResolutionChanged(FString SelectedItem, ESelectInfo::Type SelectionType)
{
    if (SelectionType == ESelectInfo::Direct && SelectedItem != PreviousSetting) {return;}
    CHECK_NULLPTR_RET(GEngine, LogUserWidget, "OptionsMenu:: GEngine is NULL!");
    
    UGameUserSettings* Settings = GEngine->GetGameUserSettings();
    CHECK_NULLPTR_RET(Settings, LogUserWidget, "OptionsMenu:: Could not retrieve Game User Settings!");
    
    FIntPoint NewRes(0);
    if (SelectedItem == FString("3840 x 2160")) {NewRes.X = 3840; NewRes.Y = 2160;}
    else if (SelectedItem == FString("2560 x 1440")) {NewRes.X = 2560; NewRes.Y = 1440;}
    else if (SelectedItem == FString("1920 x 1080")) {NewRes.X = 1920; NewRes.Y = 1080;}
    else {NewRes.X = 800; NewRes.Y = 600;}

    Settings->SetScreenResolution(NewRes);
    Settings->ApplySettings(false);

    if (SelectionType != ESelectInfo::Direct) {StartConfirmation();}
}

void UOptionsMenu::OnResolutionOpened()
{
    CHECK_NULLPTR_RET(ResolutionDropdown, LogUserWidget, "OptionsMenu:: No ResolutionDropdown!");
    PreviousSetting = ResolutionDropdown->GetSelectedOption();
}

void UOptionsMenu::StartConfirmation()
{
    if (OptionsContainer != nullptr) {OptionsContainer->SetVisibility(ESlateVisibility::Hidden);}
    if (ConfirmationBox != nullptr) {ConfirmationBox->SetVisibility(ESlateVisibility::Visible);}

    ConfirmationStartTime = CurrentTime;
    bAwaitingConfirmation = true;
}

void UOptionsMenu::EndConfirmation(bool bConfirmed)
{
    if (!bConfirmed) {if (ResolutionDropdown != nullptr) {ResolutionDropdown->SetSelectedOption(PreviousSetting);}}

    if (OptionsContainer != nullptr) {OptionsContainer->SetVisibility(ESlateVisibility::Visible);}
    if (ConfirmationBox != nullptr) {ConfirmationBox->SetVisibility(ESlateVisibility::Hidden);}

    bAwaitingConfirmation = false;
}

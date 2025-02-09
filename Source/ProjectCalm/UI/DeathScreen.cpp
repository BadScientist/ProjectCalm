#include "DeathScreen.h"
#include "MenuInterface.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"


bool UDeathScreen::Initialize()
{
    bool Success = Super::Initialize();
    if (!Success) {return false;}

    CHECK_NULLPTR_RETVAL(RespawnButton, LogUserWidget, "DeathScreen:: No RespawnButton in Widget Blueprint!", false);
    RespawnButton->OnClicked.AddDynamic(this, &UDeathScreen::Respawn);
    RespawnButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    CHECK_NULLPTR_RETVAL(QuitButton, LogUserWidget, "DeathScreen:: No QuitButton in Widget Blueprint!", false);
    QuitButton->OnClicked.AddDynamic(this, &UDeathScreen::QuitToMainMenu);
    QuitButton->OnHovered.AddDynamic(this, &UMenu::PlayButtonHoverSound);

    UWorld* World = GetWorld();
    InitTimeStamp = World == nullptr ? 0.0f : World->GetTimeSeconds();

    return true;
}

void UDeathScreen::NativeTick(const FGeometry &MyGeometry, float InDeltaTime)
{
    float CheckpointTime = BackgroundFadeTime;
    UWorld* World = GetWorld();
    float CurrentTime = World == nullptr ? CheckpointTime + InitTimeStamp : World->GetTimeSeconds();

    float BackgroundAlpha = FMath::Min((CurrentTime - InitTimeStamp) / CheckpointTime, 1.0f);
    if (Background != nullptr) {Background->SetRenderOpacity(FMath::Lerp(0.0f, 1.0f, BackgroundAlpha));}
    CheckpointTime += ContentsFadeDelay;

    if (CurrentTime < InitTimeStamp + CheckpointTime)
    {
        UE_LOG(LogTemp, Display, TEXT("InitTime: %f, CurrentTime: %f, BackgroundAlpha: %f"), InitTimeStamp, CurrentTime, BackgroundAlpha);
        return;
    }
    float LastCheckpointTime = CheckpointTime;
    CheckpointTime += ContentsFadeTime;

    float ContentsAlpha = FMath::Min((CurrentTime - (InitTimeStamp + LastCheckpointTime)) / (CheckpointTime - LastCheckpointTime), 1.0f);
    if (Contents != nullptr) {Contents->SetRenderOpacity(FMath::Lerp(0.0f, 1.0f, ContentsAlpha));}
    CheckpointTime += ButtonsRevealDelay;

    if (CurrentTime < InitTimeStamp + CheckpointTime)
    {
        UE_LOG(LogTemp, Display, TEXT("InitTime: %f, CurrentTime: %f, BackgroundAlpha: %f, ContentsAlpha: %f"), InitTimeStamp, CurrentTime, BackgroundAlpha, ContentsAlpha);
        return;
    }
    if (Buttons != nullptr) {Buttons->SetVisibility(ESlateVisibility::SelfHitTestInvisible);}

    UE_LOG(LogTemp, Display, TEXT("InitTime: %f, CurrentTime: %f, BackgroundAlpha: %f, ContentsAlpha: %f, FinalCheckpoint: %f"), InitTimeStamp, CurrentTime, BackgroundAlpha, ContentsAlpha, CheckpointTime);
}

void UDeathScreen::Setup(bool bIsInteractiveIn)
{
    Super::Setup(bIsInteractiveIn);

    if (!bIsInteractive) {return;}

    APlayerController* PlayerController = GetOwningPlayer();
    CHECK_NULLPTR_RET(PlayerController, LogUserWidget, "Menu:: PlayerController is NULL!");

    SetIsFocusable(true);
    FInputModeUIOnly InputModeData;
    InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
    InputModeData.SetWidgetToFocus(TakeWidget());
    PlayerController->SetInputMode(InputModeData);
    PlayerController->bShowMouseCursor = true;
}

void UDeathScreen::Teardown()
{
    Super::Teardown();
    
    if (bIsInteractive) {SetGameOnlyControls();}
}

void UDeathScreen::Respawn()
{
    PlayRespawnButtonPressedSound();

    if (AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this))
    {
        GameMode->OnPlayerRespawn.Broadcast();
    }

    Teardown();    
}

void UDeathScreen::QuitToMainMenu()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "PauseMenu:: No MenuInterface!");
    PlayButtonPressedSound(true);
    MenuInterface->QuitToMainMenu();
}

void UDeathScreen::PlayRespawnButtonPressedSound()
{
    CHECK_NULLPTR_RET(MenuInterface, LogUserWidget, "Menu:: MenuInterface is NULL!");
    MenuInterface->PlayUISound("UIStartButtonPressed", this, true);
}

void UDeathScreen::SetCauseOfDeathText(FString InString)
{
    if (CauseOfDeathText != nullptr) {CauseOfDeathText->SetText(FText::FromString(InString));}
}

// Copyright 2025 Joseph D Tong aka "BadScientist"


#include "PCPlayerController.h"

#include "Widgets/SViewport.h"


void FInputModePCGameOnly::ApplyInputMode(FReply& SlateOperations, UGameViewportClient& GameViewportClient) const
{
	TSharedPtr<SViewport> ViewportWidget = GameViewportClient.GetGameViewportWidget();
	if (ViewportWidget.IsValid())
	{
		TSharedRef<SViewport> ViewportWidgetRef = ViewportWidget.ToSharedRef();
		SlateOperations.UseHighPrecisionMouseMovement(ViewportWidgetRef);
		SlateOperations.SetUserFocus(ViewportWidgetRef);
		SlateOperations.LockMouseToWidget(ViewportWidgetRef);
		GameViewportClient.SetMouseLockMode(EMouseLockMode::LockAlways);
		GameViewportClient.SetIgnoreInput(false);
		GameViewportClient.SetHideCursorDuringCapture(true);
		GameViewportClient.SetMouseCaptureMode(EMouseCaptureMode::CapturePermanently_IncludingInitialMouseDown);
	}
}

#if UE_ENABLE_DEBUG_DRAWING
const FString& FInputModePCGameOnly::GetDebugDisplayName() const
{
	static const FString DebugName = TEXT("PC Game Only (Input only consumed by the player, mouse locked and permanently captured)");
	return DebugName;
}
#endif	// UE_ENABLE_DEBUG_DRAWING

APCPlayerController::APCPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{

}

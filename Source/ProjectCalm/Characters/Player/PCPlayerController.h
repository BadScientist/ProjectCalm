// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PCPlayerController.generated.h"


struct FInputModePCGameOnly : public FInputModeDataBase
{

	FInputModePCGameOnly(){};

	virtual void ApplyInputMode(FReply& SlateOperations, class UGameViewportClient& GameViewportClient) const override;

#if UE_ENABLE_DEBUG_DRAWING
	virtual const FString& GetDebugDisplayName() const override;
#endif	// UE_ENABLE_DEBUG_DRAWING
};

/**
 * 
 */
UCLASS()
class PROJECTCALM_API APCPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	APCPlayerController(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
};

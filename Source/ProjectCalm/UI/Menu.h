// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Menu.generated.h"

class UButton;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UMenu(const FObjectInitializer& ObjectInitializer);

protected:
	bool bIsInteractive{false};

	virtual void NativeDestruct() override;
	class IMenuInterface* MenuInterface;

	void SetGameOnlyControls();

public:
	virtual void SetMenuInterface(IMenuInterface* NewMenuInterface);

	virtual void Setup(bool bIsInteractiveIn = true);

	UFUNCTION()
	virtual void Teardown();

	UFUNCTION()
	void PlayButtonHoverSound();
	UFUNCTION()
	void PlayButtonPressedSound(bool bPersistOnLevelLoad = false);
	
};

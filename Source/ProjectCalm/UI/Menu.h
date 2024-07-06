// Fill out your copyright notice in the Description page of Project Settings.

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
	void SetMenuInterface(IMenuInterface* NewMenuInterface);

	virtual void Setup(bool bIsInteractiveIn = true);

	UFUNCTION()
	virtual void Teardown();
	
};

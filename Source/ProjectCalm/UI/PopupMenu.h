// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PopupWidget.h"
#include "PopupMenu.generated.h"

class UInputMappingContext;
class UInputAction;
struct FEnhancedInputActionEventBinding;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPopupMenu : public UPopupWidget
{
	GENERATED_BODY()

public:
	UPopupMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

public:
	virtual void Setup(bool bIsInteractiveIn = true) override;
    virtual void Teardown() override;
	
};

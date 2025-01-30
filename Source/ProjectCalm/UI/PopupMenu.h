// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "PopupMenu.generated.h"

class UInputMappingContext;
class UInputAction;
struct FEnhancedInputActionEventBinding;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPopupMenu : public UMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;
	
	virtual FReply NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent);

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

protected:
	UFUNCTION()
	virtual void OnCloseButtonClicked();

public:
	virtual void Setup(bool bIsInteractiveIn = true) override;
    virtual void Teardown() override;
	
};

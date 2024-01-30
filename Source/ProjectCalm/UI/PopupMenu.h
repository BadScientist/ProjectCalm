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

public:
	UPopupMenu(const FObjectInitializer& ObjectInitializer);

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* MenuMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CloseAction;

	FEnhancedInputActionEventBinding* ActionBinding;

	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	void CloseMenu();

public:
	virtual void Setup(bool bIsInteractiveIn = true) override;
	virtual void Teardown() override;
	
};

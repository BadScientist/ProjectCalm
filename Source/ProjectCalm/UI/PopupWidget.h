// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "PopupWidget.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPopupWidget : public UMenu
{
	GENERATED_BODY()

public:
	UPopupWidget(const FObjectInitializer& ObjectInitializer);

protected:
	UFUNCTION(BlueprintCallable, meta = (AllowPrivateAccess = "true"))
	virtual void CloseMenu();
	
};

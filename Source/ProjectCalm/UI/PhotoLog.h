// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "PopupMenu.h"
#include "PhotoLog.generated.h"

class UImage;
class UButton;
struct FPhotoData;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPhotoLog : public UPopupMenu
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget))
    UImage* PhotoDisplay;
	UPROPERTY(meta = (BindWidget))
    UButton* LeftButton;
	UPROPERTY(meta = (BindWidget))
    UButton* RightButton;

    TArray<FPhotoData> Photos;

public:
	virtual void Setup(bool bIsInteractiveIn = true) override;
	
};

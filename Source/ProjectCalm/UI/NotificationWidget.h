// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "NotificationWidget.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogNotificationWidget, All, All)

class UTextBlock;
class UImage;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UNotificationWidget : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* NotificationText;
	UPROPERTY(meta = (BindWidget))
	UImage* NotificationBackground;

public:
	void DisplayString(FString InString);
    void SetAlpha(float InAlpha);

};

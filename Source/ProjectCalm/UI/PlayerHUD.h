// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUD.generated.h"

class UNotificationWidget;
class UTextBlock;
class UImage;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

	UPROPERTY(meta = (BindWidget))
	UTextBlock* InteractionLabelText;
	UPROPERTY(meta = (BindWidget))
	UImage* Reticle;
	UPROPERTY(meta = (BindWidget))
	UNotificationWidget* NotificationWidget;

public:
	UPlayerHUD(const FObjectInitializer& ObjectInitializer);

	void Hide();
	void Show();
	void SetInteractionLabelText(FString InString);

	UNotificationWidget* GetNotificationWidget() const {return NotificationWidget;}
	UTextBlock* GetInteractionLabelText() const {return InteractionLabelText;}
	
};

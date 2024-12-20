// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "DeathScreen.generated.h"

class UButton;
class UImage;
class UVerticalBox;
class UHorizontalBox;
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UDeathScreen : public UMenu
{
	GENERATED_BODY()

private:
	UPROPERTY(meta = (BindWidget))
	UButton* RespawnButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitButton;
	UPROPERTY(meta = (BindWidget))
	UImage* Background;
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Contents;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Buttons;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* CauseOfDeathText;

	UPROPERTY(EditDefaultsOnly, Category = FadeIn, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BackgroundFadeTime{2.0f};
	UPROPERTY(EditDefaultsOnly, Category = FadeIn, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ContentsFadeDelay{1.0f};
	UPROPERTY(EditDefaultsOnly, Category = FadeIn, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ContentsFadeTime{2.0f};
	UPROPERTY(EditDefaultsOnly, Category = FadeIn, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float ButtonsRevealDelay{1.0f};

	float InitTimeStamp{0.0f};

protected:
	virtual bool Initialize() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime);

public:
	virtual void Setup(bool bIsInteractiveIn) override;
	virtual void Teardown() override;

private:
	UFUNCTION()
	void Respawn();
    
	UFUNCTION()
	void QuitToMainMenu();

	UFUNCTION(BlueprintCallable)
	void PlayRespawnButtonPressedSound();

public:
	void SetCauseOfDeathText(FString InString);
	
};

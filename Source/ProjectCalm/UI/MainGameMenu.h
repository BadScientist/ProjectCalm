// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Menu.h"
#include "MainGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UMainGameMenu : public UMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize() override;

private:
	UPROPERTY(meta = (BindWidget))
	class UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
	class UButton* ExitButton;

	UFUNCTION()
	void StartGame();
	UFUNCTION()
	void ExitGame();

public:
	virtual void Setup(bool bIsInteractiveIn) override;
	virtual void Teardown() override;
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

class UPopupWidget;


UINTERFACE(MinimalAPI)
class UMenuInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API IMenuInterface
{
	GENERATED_BODY()

public:
	virtual void StartGame() = 0;
	virtual void QuitToMainMenu() = 0;
	virtual void QuitToDesktop() = 0;
	virtual void PlayUISound(FName SoundName, UObject *WorldContextObject, bool bPersistOnLevelLoad = false) = 0;
	virtual float GetMasterVolume() = 0;
	virtual float GetMusicVolume() = 0;
	virtual float GetAmbientVolume() = 0;
	virtual float GetSFXVolume() = 0;
	virtual float GetUIVolume() = 0;
	virtual void SetMasterVolume(float InVolume) = 0;
	virtual void SetMusicVolume(float InVolume) = 0;
	virtual void SetAmbientVolume(float InVolume) = 0;
	virtual void SetSFXVolume(float InVolume) = 0;
	virtual void SetUIVolume(float InVolume) = 0;
};

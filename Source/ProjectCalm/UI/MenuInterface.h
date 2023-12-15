// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "MenuInterface.generated.h"

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
};

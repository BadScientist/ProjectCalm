// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "PausableInterface.generated.h"

class UPopupWidget;


UINTERFACE(MinimalAPI)
class UPausableInterface : public UInterface
{
	GENERATED_BODY()
};


class PROJECTCALM_API IPausableInterface
{
	GENERATED_BODY()

public:
	virtual void OnGamePaused(float InTimeDilation) = 0;
	virtual void OnGameUnpaused(float InTimeDilation) = 0;
};

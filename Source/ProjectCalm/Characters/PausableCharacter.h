// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ProjectCalm/Game/PausableInterface.h"
#include "PausableCharacter.generated.h"


UCLASS(config = game)
class PROJECTCALM_API APausableCharacter : public ACharacter, public IPausableInterface
{
	GENERATED_BODY()

    public:
	// Default constructor
	APausableCharacter(const FObjectInitializer& ObjectInitializer);

protected:
    virtual void BeginPlay() override;

	// START IPAUSABLEINTERFACE
	UFUNCTION()
	virtual void OnGamePaused(float InTimeDilation) override;
	UFUNCTION()
	virtual void OnGameUnpaused(float InTimeDilation) override;
	// END IPAUSABLEINTERFACE

};

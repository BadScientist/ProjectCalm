// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "PCPerceptionStimulusComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPCPerceptionStimulusComponent : public UAIPerceptionStimuliSourceComponent
{
	GENERATED_BODY()

public:
	UPCPerceptionStimulusComponent(const FObjectInitializer &ObjectInitializer);

protected:
	virtual void OnRegister() override;
};

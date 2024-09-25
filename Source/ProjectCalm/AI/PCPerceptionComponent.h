// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "PCPerceptionComponent.generated.h"

/**
 * 
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPCPerceptionComponent : public UAIPerceptionComponent
{
	GENERATED_BODY()

public:
	UPCPerceptionComponent(const FObjectInitializer &ObjectInitializer);
	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "QuadrupedPawnMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ECustomMovementMode : uint8
{
	/** Walking on a surface. */
	CUSTOM_MOVE_QuadrupedWalking	UMETA(DisplayName="QuadrupedWalking")
};


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UQuadrupedPawnMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()
	
};

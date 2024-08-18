// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"
#include "SpecialRock.generated.h"


USTRUCT(BlueprintType)
struct FDestination
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector Location{FVector::ZeroVector};
	UPROPERTY(EditAnywhere)
	FRotator Rotation{FRotator::ZeroRotator};
};

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ASpecialRock : public AInteractableActor
{
	GENERATED_BODY()

public:
	ASpecialRock();
	ASpecialRock(const FObjectInitializer& ObjectInitializer) : ASpecialRock(){};

private:
	UPROPERTY(EditAnywhere, Category = SpecialRock)
	TArray<FDestination> TeleportLocations;

protected:
	virtual void BeginPlay() override;

public:
	/*START INTERACTABLE INTERFACE*/
    virtual void Interact(APlayerCharacter* InteractingPlayer);
	/*END INTERACTABLE INTERFACE*/

	
};

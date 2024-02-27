// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"

#include "Pickup.generated.h"

class APlayerCharacter;
class UItemData;


UCLASS()
class PROJECTCALM_API APickup : public AInteractableActor
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, Category = Contents)
	TArray<UItemData*> Items;
	
public:	
	// Sets default values for this actor's properties
	APickup();

	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

private:
	void AbortPickup(APlayerCharacter* InteractingPlayer);

};

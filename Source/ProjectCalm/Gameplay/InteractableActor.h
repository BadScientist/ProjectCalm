// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableInterface.h"
#include "InteractableActor.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteractable, All, All)

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;


UCLASS()
class PROJECTCALM_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FString InteractionLabel{FString("Activate")};

    APlayerCharacter* PlayerCharacter{nullptr};
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor(){};

	// START INTERACTABLEINTERFACE
	virtual FString GetInteractionLabel() const override {return InteractionLabel;};
    virtual void Interact(APlayerCharacter* InteractingPlayer);
	// END INTERACTABLEINTERFACE

};

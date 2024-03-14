// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableInterface.h"
#include "InteractableActor.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;


UCLASS()
class PROJECTCALM_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

protected:    
	UPROPERTY(EditDefaultsOnly, Category = Mesh, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* InteractionMesh{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = Interaction, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* InteractionCollision{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FString InteractionLabel{FString("Activate")};

	ECollisionEnabled::Type ActiveCollisionType{ECollisionEnabled::QueryOnly};
	ECollisionEnabled::Type InactiveCollisionType{ECollisionEnabled::NoCollision};

    APlayerCharacter* PlayerCharacter{nullptr};
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor(){};

	// START INTERACTABLEINTERFACE
	virtual FString GetInteractionLabel() const override {return InteractionLabel;};
    virtual void Interact(APlayerCharacter* InteractingPlayer);
	// END INTERACTABLEINTERFACE

	void SetCollisionEnabled(bool bValue);

};

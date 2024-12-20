// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "InteractableInterface.h"
#include "InteractableActor.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class USoundCue;


UCLASS()
class PROJECTCALM_API AInteractableActor : public AActor, public IInteractableInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Mesh)
	UStaticMeshComponent* InteractionMesh{nullptr};

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Interaction)
	UBoxComponent* InteractionCollision{nullptr};

	UPROPERTY(EditDefaultsOnly, Category = Interaction)
	FString InteractionLabel{FString("Activate")};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = SFX)
	USoundCue* InteractionSound{nullptr};

	ECollisionEnabled::Type ActiveCollisionType{ECollisionEnabled::QueryOnly};
	ECollisionEnabled::Type InactiveCollisionType{ECollisionEnabled::NoCollision};

    APlayerCharacter* PlayerCharacter{nullptr};
	int32 TimesInteracted{0};
	
public:	
	// Sets default values for this actor's properties
	AInteractableActor();
	AInteractableActor(const FObjectInitializer& ObjectInitializer) : AInteractableActor(){};

	// START INTERACTABLEINTERFACE
	virtual FString GetInteractionLabel() const override {return InteractionLabel;};
    virtual void Interact(APlayerCharacter* InteractingPlayer);
	// END INTERACTABLEINTERFACE

	void SetCollisionEnabled(bool bValue);

protected:
	void SetCollisionProfile(FName ProfileName);

};

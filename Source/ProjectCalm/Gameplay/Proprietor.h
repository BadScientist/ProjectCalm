// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InteractableActor.h"

#include "Proprietor.generated.h"

class APlayerCharacter;
class UBoxComponent;
class UStaticMeshComponent;
class UArrowComponent;


UCLASS()
class PROJECTCALM_API AProprietor : public AInteractableActor
{
	GENERATED_BODY()
    
	UPROPERTY(EditDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* ProprietorMesh{nullptr};
	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* ProprietorCollision{nullptr};
	
public:	
	// Sets default values for this actor's properties
	AProprietor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

    void SetCollisionEnabled(bool bValue);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestGiver.h"

#include "Proprietor.generated.h"

class APlayerCharacter;


UCLASS()
class PROJECTCALM_API AProprietor : public AQuestGiver
{
	GENERATED_BODY()
	
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

};

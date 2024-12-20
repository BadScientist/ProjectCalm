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

	UPROPERTY(EditAnywhere, Category = Motion, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MoveSpeed{40};
	UPROPERTY(EditAnywhere, Category = Motion, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MoveSpeedVariation{10};
	UPROPERTY(EditAnywhere, Category = Motion, meta=(ClampMin="0", UIMin="0", ForceUnits="cm"))
	float MaxZOffset{15};
	UPROPERTY(EditAnywhere, Category = Motion, meta=(ClampMin="0", UIMin="0", ForceUnits="cm"))
	float MinZOffset{5};

	float LastZOffset{0};
	float CurrentSpeed{10};
	float TargetZOffset{0};	
	float Progress{0};
	
public:	
	// Sets default values for this actor's properties
	AProprietor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	// START INTERACTABLEINTERFACE
    virtual void Interact(APlayerCharacter* InteractingPlayer) override;
	// END INTERACTABLEINTERFACE

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoiseMakerComponent.generated.h"

class UCharacterMovementComponent;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UNoiseMakerComponent : public UActorComponent
{
	GENERATED_BODY()

	UCharacterMovementComponent* OwnerMovementComp;

public:	
	// Sets default values for this component's properties
	UNoiseMakerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** Makes noise based on the Character's mass and speed. */
	void MakeMovementNoise();

		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseMakerComponent.h"
#include "ProjectCalmGameMode.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UNoiseMakerComponent::UNoiseMakerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}

void UNoiseMakerComponent::MakeMovementNoise(float NoiseLevel)
{
	AProjectCalmGameMode* GameMode = Cast<AProjectCalmGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode == nullptr) {return;}

	if (!FMath::IsNearlyZero(NoiseLevel)) {GameMode->OnMakeNoise.Broadcast(GetOwner(), NoiseLevel);}
}

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
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNoiseMakerComponent::BeginPlay()
{
	Super::BeginPlay();
	
	if (ACharacter* OwnerCharacter = Cast<ACharacter>(GetOwner()))
	{
		OwnerMovementComp = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
	}
}


// Called every frame
void UNoiseMakerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	MakeMovementNoise();
}

void UNoiseMakerComponent::MakeMovementNoise()
{
	AProjectCalmGameMode* GameMode = Cast<AProjectCalmGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode == nullptr || OwnerMovementComp == nullptr) {return;}

	float Speed = OwnerMovementComp->Velocity.Size();
	float NoiseLevel = OwnerMovementComp->Mass * Speed / 500;

	if (!FMath::IsNearlyZero(NoiseLevel)) {GameMode->OnMakeNoise.Broadcast(GetOwner(), NoiseLevel);}
}

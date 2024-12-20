// Fill out your copyright notice in the Description page of Project Settings.


#include "NoiseListenerComponent.h"
#include "ProjectCalm/Game/ProjectCalmGameMode.h"
#include "ProjectCalm/AI/PhotoSubjectAIController.h"
#include "ProjectCalm/AI/AlertLevel.h"

#include "Kismet/GameplayStatics.h"

#define ATTENUATION_FACTOR 10/3


// Sets default values for this component's properties
UNoiseListenerComponent::UNoiseListenerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UNoiseListenerComponent::BeginPlay()
{
	Super::BeginPlay();

	AProjectCalmGameMode* GameMode = Cast<AProjectCalmGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode == nullptr) {return;}

	GameMode->OnMakeNoise.AddDynamic(this, &UNoiseListenerComponent::OnNoiseHeard);
	
}


// Called every frame
void UNoiseListenerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNoiseListenerComponent::OnNoiseHeard(AActor *SourceActor, float NoiseLevel)
{
	if (GetOwner() == SourceActor) {return;}
	
	float DistanceToSource = FVector::Distance(SourceActor->GetActorLocation(), GetOwner()->GetActorLocation()) / 100;
	float AttenuatedNoise = NoiseLevel - ATTENUATION_FACTOR * sqrt(DistanceToSource);

	// UE_LOG(LogTemp, Warning, TEXT("NoiseListenerComponent::%s received %f noise, attenuated to %f, from %s at %f m."),
	// 	*GetOwner()->GetActorNameOrLabel(), NoiseLevel, AttenuatedNoise, *SourceActor->GetActorNameOrLabel(), DistanceToSource);
	
	if (AttenuatedNoise < AlertNoiseThreshold) {return;}

	EAlertLevel AlertLevel = AttenuatedNoise >= AlarmNoiseThreshold ? EAlertLevel::ALARMED : EAlertLevel::ALERT;
	ReactToNoise(SourceActor, AlertLevel);
}

void UNoiseListenerComponent::ReactToNoise(AActor *SourceActor, EAlertLevel AlertLevel)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (OwnerPawn == nullptr) {return;}

	APhotoSubjectAIController* OwnerController = Cast<APhotoSubjectAIController>(OwnerPawn->GetController());
	if (OwnerController == nullptr) {return;}

	// UE_LOG(LogTemp, Warning, TEXT("NoiseListenerComponent::%s reacting to %s."), *GetOwner()->GetActorNameOrLabel(), *SourceActor->GetActorNameOrLabel());
	// OwnerController->DetermineReaction(AlertLevel, SourceActor);
}

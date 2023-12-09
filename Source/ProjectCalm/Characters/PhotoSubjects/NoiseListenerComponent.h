// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NoiseListenerComponent.generated.h"

enum EAlertLevel;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UNoiseListenerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNoiseListenerComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = Reaction)
	float AlertNoiseThreshold {20};
	UPROPERTY(EditAnywhere, Category = Reaction)
	float AlarmNoiseThreshold {30};

	UFUNCTION()
	void OnNoiseHeard(AActor* SourceActor, float NoiseLevel);
	void ReactToNoise(AActor* SourceActor, EAlertLevel AlertLevel);
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnRegionVisualizerComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API USpawnRegionVisualizerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnRegionVisualizerComponent();

private:
    UPROPERTY(VisibleAnywhere)
    FVector RegionLocation = FVector::ZeroVector;
    UPROPERTY(VisibleAnywhere)
    FRotator RegionRotation = FRotator::ZeroRotator;
    UPROPERTY(VisibleAnywhere)
    FVector RegionSize = FVector::ZeroVector;

public:
    void UpdateProperties(FVector InLocation, FRotator InRotation, FVector InSize);
    FVector GetRegionLocation() const {return RegionLocation;}
    FRotator GetRegionRotation() const {return RegionRotation;}
    FVector GetRegionSize() const {return RegionSize;}
};

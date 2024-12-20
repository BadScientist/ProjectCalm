// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PhotoDataCollectorComponent.generated.h"

class UCameraComponent;
class USpawnerComponent;
struct FPhotoData;
struct FConvexVolume;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoDataCollectorComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhotoDataCollectorComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
    USpawnerComponent* Spawner;

public:
	FPhotoData CollectPhotoData(FConvexVolume ViewFrustum, FVector ViewLocation, FVector ViewForward);
		
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "PhotoSubjectName.h"
#include "PhotoSubjectDataComponent.generated.h"

struct FPhotoSubjectPointOfInterest;
struct FPhotoSubjectData;
struct FConvexVolume;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UPhotoSubjectDataComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPhotoSubjectDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<ESubjectName> SubjectName;
	UPROPERTY(EditAnywhere)
	TArray<FPhotoSubjectPointOfInterest> PointsOfInterest;

public:
	ESubjectName GetSubjectName() {return SubjectName;}
	bool GeneratePhotoSubjectData(FConvexVolume ViewFrustum, FVector CameraLocation, FPhotoSubjectData &OutSubjectData);
};

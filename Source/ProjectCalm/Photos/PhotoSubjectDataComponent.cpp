// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectDataComponent.h"
#include "PhotoSubjectData.h"
#include "PhotoSubjectPointOfInterest.h"

// Sets default values for this component's properties
UPhotoSubjectDataComponent::UPhotoSubjectDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UPhotoSubjectDataComponent::BeginPlay()
{
	Super::BeginPlay();
	
}

bool UPhotoSubjectDataComponent::GeneratePhotoSubjectData(FConvexVolume ViewFrustum, FVector CameraLocation, FPhotoSubjectData& OutSubjectData)
{
	bool Result = false;
	OutSubjectData.Name = SubjectName;
	if (AActor* Owner = GetOwner()) {OutSubjectData.Location = Owner->GetActorLocation();}

	UWorld* World = GetWorld();
	if (World == nullptr) {return Result;}

	FHitResult OutHit;
	FQuat Rotation = FQuat::Identity;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
	for (int32 i = 0; i < PointsOfInterest.Num(); i++)
	{
		FVector PointLocation = PointsOfInterest[i].RelativeLocation + OutSubjectData.Location;

		PointsOfInterest[i].IsOnCamera = ViewFrustum.IntersectPoint(PointLocation);
		if (!PointsOfInterest[i].IsOnCamera) {continue;}

		PointsOfInterest[i].IsInLineOfSight = !(World->LineTraceSingleByChannel(OutHit, CameraLocation, PointLocation, TraceChannel));
		
		if (PointsOfInterest[i].IsVisible())
		{
			OutSubjectData.PointsOfInterest.Add(PointsOfInterest[i]);
			Result = true;
		}
	}

    return Result;
}

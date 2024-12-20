// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoSubjectDataComponent.h"
#include "PhotoSubjectData.h"
#include "PhotoSubjectPointOfInterest.h"
#include "ProjectCalm/AI/PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "GameFramework/Character.h"

#ifdef PC_DEBUG_LOGS
	#define LOCAL_DEBUG_LOGS
#endif


// Sets default values for this component's properties
UPhotoSubjectDataComponent::UPhotoSubjectDataComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}


// Called when the game starts
void UPhotoSubjectDataComponent::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Owner = Cast<ACharacter>(GetOwner()))
	{
		if (APhotoSubjectAIController* OwnerController = Cast<APhotoSubjectAIController>(Owner->GetController()))
		{
			OwnerController->SetSelfSpecies(SubjectName);
		}
	}
	
}

bool UPhotoSubjectDataComponent::GeneratePhotoSubjectData(FConvexVolume ViewFrustum, FCameraOrientation CameraOrientation, FPhotoSubjectData& OutSubjectData)
{
	bool Result{false};
	float Score{0};
	OutSubjectData.Name = SubjectName;

	AActor* Owner = GetOwner();
	CHECK_NULLPTR_RETVAL(Owner, LogActorComponent, "PhotoSubjectDataComponent:: No Owning Actor!", false);

	OutSubjectData.Location = Owner->GetActorLocation();
	float Distance = FVector::Distance(OutSubjectData.Location, CameraOrientation.Location);
	float DistanceMultiplier = FMath::Max(1.5f - (FMath::Abs(IdealPhotoDistance - Distance) / 1000), 0.5f);

	FVector Direction = (OutSubjectData.Location - CameraOrientation.Location).GetSafeNormal();
	float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Direction, CameraOrientation.Forward)) - PI / 2);
	float AngleMultiplier = FMath::Max(1.5f - (Angle / 30), 0.5f);

	if (ACharacter* OwningCharacter = Cast<ACharacter>(GetOwner()))
	{
		if (APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwningCharacter->GetController()))
		{
			OutSubjectData.Behavior = AIController->GetActiveBehavior();
		}
	}

	UWorld* World = GetWorld();
	if (World == nullptr) {return Result;}

	float BaseScore{0};
	FHitResult OutHit;
	FQuat Rotation = FQuat::Identity;
	ECollisionChannel TraceChannel = ECollisionChannel::ECC_Visibility;
	for (int32 i = 0; i < PointsOfInterest.Num(); i++)
	{
		FVector PointLocation = PointsOfInterest[i].RelativeLocation + OutSubjectData.Location;

		PointsOfInterest[i].IsOnCamera = ViewFrustum.IntersectPoint(PointLocation);
		if (!PointsOfInterest[i].IsOnCamera) {continue;}

		PointsOfInterest[i].IsInLineOfSight = !(World->LineTraceSingleByChannel(OutHit, CameraOrientation.Location, PointLocation, TraceChannel));
		
		if (PointsOfInterest[i].IsVisible())
		{
			OutSubjectData.PointsOfInterest.Add(PointsOfInterest[i]);
			BaseScore += PointsOfInterest[i].ScoreValue;
			Result = true;
		}
	}

	OutSubjectData.Score = BaseScore * DistanceMultiplier * AngleMultiplier;

#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Warning, TEXT("PhotoSubjectDataComponent:: Subject:  %s"), *(PCSubjectName::EnumToString(OutSubjectData.Name)));
	UE_LOG(LogActorComponent, Warning, TEXT("PhotoSubjectDataComponent:: Distance:  %f | DistanceMultiplier: %f"), Distance, DistanceMultiplier);
	UE_LOG(LogActorComponent, Warning, TEXT("PhotoSubjectDataComponent:: Angle:     %f | AngleMultiplier:    %f"), Angle, AngleMultiplier);
	UE_LOG(LogActorComponent, Warning, TEXT("PhotoSubjectDataComponent:: BaseScore: %f | FinalScore:         %f"), BaseScore, OutSubjectData.Score);
#endif

    return Result;
}

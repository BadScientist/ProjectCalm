// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetLocation.h"
#include "PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

#ifdef PC_DEBUG_DRAW_SHAPES
	// #define LOCAL_DEBUG_DRAW_SHAPES
#endif


float UBTTask_GetLocation::CalculateVectorDistance2D(FVector VectorA, FVector VectorB)
{        
    return FVector::Distance(FVector(VectorA.X, VectorA.Y, 0.0f), FVector(VectorB.X, VectorB.Y, 0.0f));
}

bool UBTTask_GetLocation::FindGround(FVector InLocation, FHitResult& OutHit)
{
    UWorld *World = GetWorld();
    bool bHit = World->LineTraceSingleByChannel(OutHit, InLocation, InLocation + FVector::DownVector * 3000, ECollisionChannel::ECC_WorldStatic);
    if (!bHit) {bHit = World->LineTraceSingleByChannel(OutHit, InLocation + FVector::UpVector * 3001, InLocation, ECollisionChannel::ECC_WorldStatic);}
    
    return bHit;
}

FVector UBTTask_GetLocation::TranslateToRandomElevation(FVector InLocation)
{
    FHitResult OutHit;
    bool bHit = FindGround(InLocation, OutHit);
    if (!bHit) {return InLocation;}

    float GroundElevation = OutHit.Location.Z;
    float CurrentElevation = InLocation.Z;
    if (GroundElevation + MaxDistanceFromGround < CurrentElevation - MaxElevationChange) {return InLocation;}

    float NewElevation = FMath::RandRange(
        FMath::Clamp(CurrentElevation - MaxElevationChange, GroundElevation + MinDistanceFromGround, GroundElevation + MaxDistanceFromGround),
        FMath::Clamp(CurrentElevation + MaxElevationChange, GroundElevation + MinDistanceFromGround, GroundElevation + MaxDistanceFromGround));
    return FVector(OutHit.Location.X, OutHit.Location.Y, NewElevation);
}

bool UBTTask_GetLocation::ProjectLocationToSurface(FVector InLocation, FVector& OutLocation)
{
    FHitResult OutHit;
    bool bHit = FindGround(InLocation, OutHit);
    if (!bHit) 
    {
#ifdef LOCAL_DEBUG_LOGS
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_GetLocation:: Could not find ground!"));
#endif
        return false;
    }

    const UNavigationSystemBase* NavSystem = GetWorld()->GetNavigationSystem();
    const UNavigationSystemV1* NavSystemV1 = Cast<UNavigationSystemV1>(NavSystem);
    if (!NavSystem)
    {
#ifdef LOCAL_DEBUG_LOGS
        UE_LOG(LogTemp, Warning, TEXT("UBTTask_GetLocation:: Could not find Nav System!"));
#endif
        return false;
    }
    
    FNavLocation Result;
    bool bSucceeded = NavSystemV1->ProjectPointToNavigation(OutHit.Location, Result);
        if(!bSucceeded)
        {
#ifdef LOCAL_DEBUG_LOGS
            UE_LOG(LogTemp, Warning, TEXT("UBTTask_GetLocation:: Failed to project point to navigation!"));
#endif
#ifdef LOCAL_DEBUG_DRAW_SHAPES
            DrawDebugPoint(GetWorld(), OutHit.Location, 50, FColor::Blue, false, 4);
#endif
        }

    OutLocation = Result.Location;
    return bSucceeded;
}

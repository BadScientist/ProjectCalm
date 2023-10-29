// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetLocationInRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "NavigationSystem.h"


UBTTask_GetLocationInRange::UBTTask_GetLocationInRange()
{
    NodeName = TEXT("Get Location In Range");
}

EBTNodeResult::Type UBTTask_GetLocationInRange::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    APawn* Pawn = nullptr;
    if (AAIController* AIController = OwnerComp.GetAIOwner()) {Pawn = AIController->GetPawn();}
    if (Pawn == nullptr) 
    {
        UE_LOG(LogTemp, Error, TEXT("BTTask::GetLocationInRange:: PAWN NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    if (!OwnerComp.GetBlackboardComponent()->IsVectorValueSet(HomeLocationKey.SelectedKeyName))
    {
        UE_LOG(LogTemp, Error, TEXT("BTTask::GetLocationInRange:: HOME LOCATION NOT SET!"));
        return EBTNodeResult::Failed;
    }

    FVector PawnLocation = Pawn->GetActorLocation();
    FVector HomeLocation = OwnerComp.GetBlackboardComponent()->GetValueAsVector(HomeLocationKey.SelectedKeyName);

    FVector Destination2D = FindRandomLocationInRange(HomeLocation, PawnLocation);    
    FVector Destination3D;
    bool bHit = ProjectLocationToSurface(Destination2D, Destination3D);
    if (!bHit)
    {
        UE_LOG(LogTemp, Error, TEXT("BTTask::GetLocationInRange:: COULD NOT PROJECT LOCATION ONTO NAV MESH!"));
        return EBTNodeResult::Failed;
    }

#if WITH_EDITORONLY_DATA
    DrawDebugPoint(GetWorld(), HomeLocation, 50, FColor::Red, true);
    DrawDebugCircle(GetWorld(), PawnLocation, MaxDistanceFromPawn, 128, FColor::Purple, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0, FVector::ForwardVector, FVector::RightVector, false);
    DrawDebugCircle(GetWorld(), HomeLocation, MaxDistanceFromHome, 128, FColor::Red, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0, FVector::ForwardVector, FVector::RightVector, false);
    DrawDebugPoint(GetWorld(), Destination2D, 50, FColor::Blue, false, 4);
    DrawDebugPoint(GetWorld(), Destination3D, 50, FColor::Green, true);
    UE_LOG(LogTemp, Warning, TEXT("BTTask::GetLocationInRange::Destination Found: %s"), *Destination3D.ToCompactString());
#endif

    OwnerComp.GetBlackboardComponent()->SetValueAsVector(DestinationKey.SelectedKeyName, Destination3D);
    return EBTNodeResult::Succeeded;
}

FVector UBTTask_GetLocationInRange::FindRandomLocationInRange(FVector HomeLocation, FVector CurrentLocation)
{
    float DistanceFromHome = FVector::Distance(FVector(HomeLocation.X, HomeLocation.Y, 0.0f), FVector(CurrentLocation.X, CurrentLocation.Y, 0.0f));
    float AngleToTravel = FMath::RandRange(0.0f, 2 * PI);
    
    float MaxTravelDistance = CalculateMaxDistance(AngleToTravel, DistanceFromHome);
    float TravelDistance = FMath::RandRange(0.0f, FMath::Min(MaxTravelDistance, MaxDistanceFromPawn));
    
    FVector Direction;
    if (FMath::IsNearlyZero(DistanceFromHome)) {Direction = FVector::ForwardVector;}
    else {Direction = (HomeLocation - CurrentLocation).GetSafeNormal2D();}
    FVector NewDirection = Direction.RotateAngleAxisRad(AngleToTravel, FVector::UpVector);
    FVector TravelVector = NewDirection * TravelDistance;

#if WITH_EDITORONLY_DATA
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * MaxTravelDistance, FColor::Red, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0);
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * MaxDistanceFromPawn, FColor::Purple, false, 4, ESceneDepthPriorityGroup::SDPG_World, 75.0);
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + TravelVector, FColor::Blue, false, 4, ESceneDepthPriorityGroup::SDPG_World, 100.0);
    UE_LOG(LogTemp, Warning, TEXT("BTTask::GetLocationInRange::MaxDistance: %f, ActualDistance: %f, Direction: %s"), MaxTravelDistance, TravelDistance, *Direction.ToCompactString());
#endif

    return CurrentLocation + TravelVector;
}

bool UBTTask_GetLocationInRange::ProjectLocationToSurface(FVector InLocation, FVector& OutLocation)
{
    FHitResult OutHit;
    bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, InLocation, InLocation + FVector::DownVector * 3000, ECollisionChannel::ECC_WorldStatic);
    if (!bHit) {bHit = GetWorld()->LineTraceSingleByChannel(OutHit, InLocation + FVector::UpVector * 3000, InLocation, ECollisionChannel::ECC_WorldStatic);}
    if (!bHit) {return false;}

    UNavigationSystemBase* NavSystem = GetWorld()->GetNavigationSystem();
    UNavigationSystemV1* NavSystemV1 = Cast<UNavigationSystemV1>(NavSystem);
    if (!NavSystem) {return false;}
    
    FNavLocation Result;
    bool bSucceeded = NavSystemV1->ProjectPointToNavigation(OutHit.Location, Result);

    OutLocation = Result.Location;
    return bSucceeded;
}

float UBTTask_GetLocationInRange::CalculateMaxDistance(float TravelAngle, float DistanceFromHome)
{
    if (FMath::IsNearlyZero(MaxDistanceFromHome)) {return MaxDistanceFromPawn;}
    if (FMath::IsNearlyZero(DistanceFromHome)) {return FMath::Min(MaxDistanceFromPawn, MaxDistanceFromHome);}
    
    float AngleOfDestination = FMath::Asin(DistanceFromHome * FMath::Sin(TravelAngle) / MaxDistanceFromHome);
    float AngleOfHome = PI - AngleOfDestination - TravelAngle;
    return MaxDistanceFromHome * FMath::Sin(AngleOfHome) / FMath::Sin(TravelAngle);
}

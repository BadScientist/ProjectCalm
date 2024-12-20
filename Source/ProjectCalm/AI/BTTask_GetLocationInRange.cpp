// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetLocationInRange.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "PhotoSubjectAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

#ifdef PC_DEBUG_DRAW_SHAPES
	// #define LOCAL_DEBUG_DRAW_SHAPES
#endif


UBTTask_GetLocationInRange::UBTTask_GetLocationInRange()
{
    NodeName = TEXT("Get Location In Range");

    TObjectPtr<UBlackboardKeyType_Vector> VectorKeyType;
    HomeLocationKey.AllowedTypes = TArray<TObjectPtr<UBlackboardKeyType>>{VectorKeyType};
    HomeLocationKey.AllowNoneAsValue(false);
    DestinationKey.AllowedTypes = TArray<TObjectPtr<UBlackboardKeyType>>{VectorKeyType};
    DestinationKey.AllowNoneAsValue(false);
}

void UBTTask_GetLocationInRange::InitializeFromAsset(UBehaviorTree & Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		HomeLocationKey.ResolveSelectedKey(*BBAsset);
		DestinationKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_GetLocationInRange::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr) 
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: AI CONTROLLER NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr) 
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: PAWN NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: NO BLACKBOARD COMPONENT FOUND!"));
        return EBTNodeResult::Failed;
    }

    if (DestinationKey.SelectedKeyType.Get() != UBlackboardKeyType_Vector::StaticClass())
    {        
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: INVALID DESTINATIONKEY TYPE!"));
        return EBTNodeResult::Failed;
    }

    if (HomeLocationKey.SelectedKeyType.Get() != UBlackboardKeyType_Vector::StaticClass())
    {        
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: INVALID HOMELOCATIONKEY TYPE!"));
        return EBTNodeResult::Failed;
    }

    if (!BlackboardComponent->IsVectorValueSet(HomeLocationKey.SelectedKeyName))
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: HOME LOCATION NOT SET!"));
        return EBTNodeResult::Failed;
    }

    FVector PawnLocation = AIPawn->GetActorLocation();
    FVector HomeLocation = BlackboardComponent->GetValueAsVector(HomeLocationKey.SelectedKeyName);

    FVector Destination2D = FindRandomLocationInRange(HomeLocation, PawnLocation);
    
    FVector Destination3D;
    if(bFindPointOnGround)
    {
        bool bHit = ProjectLocationToSurface(Destination2D, Destination3D);
        if (!bHit)
        {
            UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationInRange:: COULD NOT PROJECT LOCATION ONTO NAV MESH!"));
            return EBTNodeResult::Failed;
        }
    }
    else {Destination3D = TranslateToRandomElevation(Destination2D);}

#ifdef LOCAL_DEBUG_DRAW_SHAPES
    DrawDebugPoint(GetWorld(), HomeLocation, 50, FColor::Red, true);
    DrawDebugCircle(GetWorld(), PawnLocation, MaxDistanceFromPawn, 128, FColor::Purple, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0, FVector::ForwardVector, FVector::RightVector, false);
    DrawDebugCircle(GetWorld(), HomeLocation, MaxDistanceFromHome, 128, FColor::Red, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0, FVector::ForwardVector, FVector::RightVector, false);
    DrawDebugPoint(GetWorld(), Destination2D, 50, FColor::Blue, false, 4);
    DrawDebugPoint(GetWorld(), Destination3D, 50, FColor::Green, true);
#endif
#ifdef PC_DEBUG_LOGS
    UE_LOG(LogTemp, Warning, TEXT("BTTask::GetLocationInRange::Destination Found: %s"), *Destination3D.ToCompactString());
#endif

    AIController->SetVectorKeyValue(DestinationKey.SelectedKeyName, Destination3D);
    return EBTNodeResult::Succeeded;
}

FVector UBTTask_GetLocationInRange::FindRandomLocationInRange(FVector HomeLocation, FVector CurrentLocation)
{
    float DistanceFromHome = CalculateVectorDistance2D(HomeLocation, CurrentLocation);
    float AngleToTravel = FMath::RandRange(0.0f, 2 * PI);
    
    FVector Direction = CalculateDirectionFromHome(HomeLocation, CurrentLocation);
    FVector NewDirection = Direction.RotateAngleAxisRad(AngleToTravel, FVector::UpVector);    
    
    float MaxTravelDistance = CalculateMaxDistance(AngleToTravel, DistanceFromHome);
    float TravelDistance = FMath::RandRange(0.0f, FMath::Min(MaxTravelDistance, MaxDistanceFromPawn));
    FVector TravelVector = NewDirection * TravelDistance;

#ifdef LOCAL_DEBUG_DRAW_SHAPES
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * MaxTravelDistance, FColor::Red, false, 4, ESceneDepthPriorityGroup::SDPG_World, 50.0);
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + NewDirection * MaxDistanceFromPawn, FColor::Purple, false, 4, ESceneDepthPriorityGroup::SDPG_World, 75.0);
    DrawDebugLine(GetWorld(), CurrentLocation, CurrentLocation + TravelVector, FColor::Blue, false, 4, ESceneDepthPriorityGroup::SDPG_World, 100.0);
#endif
#ifdef PC_DEBUG_LOGS
    UE_LOG(LogTemp, Warning, TEXT("BTTask::GetLocationInRange::MaxDistance: %f, ActualDistance: %f, Direction: %s"), MaxTravelDistance, TravelDistance, *Direction.ToCompactString());
#endif

    return CurrentLocation + TravelVector;
}

float UBTTask_GetLocationInRange::CalculateMaxDistance(float TravelAngle, float DistanceFromHome)
{
    if (FMath::IsNearlyZero(MaxDistanceFromHome)) {return MaxDistanceFromPawn;}
    if (FMath::IsNearlyZero(DistanceFromHome)) {return FMath::Min(MaxDistanceFromPawn, MaxDistanceFromHome);}
    
    float AngleOfDestination = FMath::Asin(DistanceFromHome * FMath::Sin(TravelAngle) / MaxDistanceFromHome);
    float AngleOfHome = PI - AngleOfDestination - TravelAngle;
    return MaxDistanceFromHome * FMath::Sin(AngleOfHome) / FMath::Sin(TravelAngle);
}

FVector UBTTask_GetLocationInRange::CalculateDirectionFromHome(FVector HomeLocation, FVector CurrentLocation)
{
    if (FMath::IsNearlyZero(CalculateVectorDistance2D(HomeLocation, CurrentLocation))) {return FVector::ForwardVector;}
    return (HomeLocation - CurrentLocation).GetSafeNormal2D();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetLocationToward.h"
#include "PhotoSubjectAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "NavigationSystem.h"


UBTTask_GetLocationToward::UBTTask_GetLocationToward()
{
    NodeName = TEXT("Get Location Toward");

    TObjectPtr<UBlackboardKeyType_Object> ObjectKeyType;
    TObjectPtr<UBlackboardKeyType_Vector> VectorKeyType;
    ReferenceKey.AllowedTypes = TArray<TObjectPtr<UBlackboardKeyType>>{ObjectKeyType, VectorKeyType};
    ReferenceKey.AllowNoneAsValue(false);
    DestinationKey.AllowedTypes = TArray<TObjectPtr<UBlackboardKeyType>>{VectorKeyType};
    DestinationKey.AllowNoneAsValue(false);
}

void UBTTask_GetLocationToward::InitializeFromAsset(UBehaviorTree &Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		ReferenceKey.ResolveSelectedKey(*BBAsset);
		DestinationKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

EBTNodeResult::Type UBTTask_GetLocationToward::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: AI CONTROLLER NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    APawn* AIPawn = AIController->GetPawn();
    if (AIPawn == nullptr) 
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: PAWN NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: BLACKBOARD COMPONENT NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    if (DestinationKey.SelectedKeyType.Get() != UBlackboardKeyType_Vector::StaticClass())
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: INVALID DESTINATIONKEY TYPE!"));
        return EBTNodeResult::Failed;
    }

    FVector ReferenceLocation;
    bool bLocationFound = GetReferenceLocation(BlackboardComponent, ReferenceLocation);
    if (!bLocationFound) {return EBTNodeResult::Aborted;}

    FVector PawnLocation = AIPawn->GetActorLocation();
    FVector ReferenceDirection = CalculateReferenceDirection(PawnLocation, ReferenceLocation);

    float RotationAngle = FMath::RandRange(-Tolerance, Tolerance);
    FVector TravelDirection = ReferenceDirection.RotateAngleAxis(RotationAngle, FVector::UpVector);

    float ReferenceDistance = CalculateVectorDistance2D(PawnLocation, ReferenceLocation);
    float MaxDistance = bAllowMovePastReference ? MaxDistanceFromPawn : FMath::Min(MaxDistanceFromPawn, ReferenceDistance);
    float TravelDistance = FMath::RandRange(MinDistanceFromPawn, FMath::Max(MinDistanceFromPawn, MaxDistance));

    FVector TravelVector = TravelDirection * TravelDistance;
    FVector TravelLocation2D = PawnLocation + TravelVector;

    FVector TravelLocation;
    if (bFindPointOnGround)
    {
        bLocationFound = ProjectLocationToSurface(TravelLocation2D, TravelLocation);
        if (!bLocationFound) {return EBTNodeResult::Failed;}
    }
    else {TravelLocation = TranslateToRandomElevation(TravelLocation2D);}
    
    AIController->SetVectorKeyValue(DestinationKey.SelectedKeyName, TravelLocation);
    return EBTNodeResult::Succeeded;
}

bool UBTTask_GetLocationToward::GetReferenceLocation(UBlackboardComponent *BlackboardComponent, FVector &OutLocation)
{
    if (ReferenceKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        AActor* ReferenceActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(ReferenceKey.SelectedKeyName));
        if (ReferenceActor == nullptr)
        {
            UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: REFERENCE OBJECT NOT AN ACTOR!"));
            return false;
        }

        OutLocation = ReferenceActor->GetActorLocation();
        return true;
    }
    else if (ReferenceKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        OutLocation = BlackboardComponent->GetValueAsVector(ReferenceKey.SelectedKeyName);
        return true;
    }
    
    UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetLocationToward:: INVALID REFERENCEKEY TYPE!"));
    return false;
}

FVector UBTTask_GetLocationToward::CalculateReferenceDirection(FVector PawnLocation, FVector ReferenceLocation)
{
    FVector ReferenceDirection = (ReferenceLocation - PawnLocation).GetSafeNormal2D();
    if (bReverseDirection) {ReferenceDirection *= -1;}
    
    return ReferenceDirection;
}

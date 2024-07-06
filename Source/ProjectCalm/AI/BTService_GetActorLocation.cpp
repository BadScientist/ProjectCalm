// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetActorLocation.h"
#include "PhotoSubjectAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"


void UBTService_GetActorLocation::InitializeFromAsset(UBehaviorTree &Asset)
{
	Super::InitializeFromAsset(Asset);

	UBlackboardData* BBAsset = GetBlackboardAsset();
	if (BBAsset)
	{
		TargetActorKey.ResolveSelectedKey(*BBAsset);
		TargetLocationKey.ResolveSelectedKey(*BBAsset);
	}
	else
	{
		UE_LOG(LogBehaviorTree, Warning, TEXT("Can't initialize task: %s, make sure that behavior tree specifies blackboard asset!"), *GetName());
	}
}

void UBTService_GetActorLocation::TickNode(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, float DeltaSeconds)
{
    Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

    StoreTargetActorLocation(OwnerComp);
}

void UBTService_GetActorLocation::StoreTargetActorLocation(UBehaviorTreeComponent& OwnerComp)
{
    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr) 
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTService::GetActorLocation:: AI CONTROLLER NOT FOUND!"));
        return;
    }

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    if (BlackboardComponent == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetActorLocation:: BLACKBOARD COMPONENT NOT FOUND!"));
        return;
    }

    if (TargetLocationKey.SelectedKeyType.Get() != UBlackboardKeyType_Vector::StaticClass())
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetActorLocation:: INVALID TARGETLOCATIONKEY TYPE!"));
        return;
    }

    if (TargetActorKey.SelectedKeyType.Get() == UBlackboardKeyType_Object::StaticClass())
    {
        AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
        if (TargetActor == nullptr)
        {
            UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetActorLocation:: REFERENCE OBJECT NOT AN ACTOR!"));
            return;
        }

        AIController->SetVectorKeyValue(TargetLocationKey.SelectedKeyName, TargetActor->GetActorLocation());
        return;
    }
    
     UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetActorLocation:: INVALID TARGETACTORKEY TYPE!"));
}

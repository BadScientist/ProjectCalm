// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_GetActorLocation.h"
#include "PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

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
    CHECK_NULLPTR_RET(AIController, LogBehaviorTree, "BTService_GetActorLocation:: Could not get AI Controller!");

    UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    CHECK_NULLPTR_RET(BlackboardComponent, LogBehaviorTree, "BTService_GetActorLocation:: Could not get Blackboard Component!");

    if (TargetLocationKey.SelectedKeyType.Get() != UBlackboardKeyType_Vector::StaticClass())
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTService_GetActorLocation:: INVALID TARGETLOCATIONKEY TYPE!"));
        return;
    }

    if (TargetActorKey.SelectedKeyType.Get() != UBlackboardKeyType_Object::StaticClass())
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::GetActorLocation:: INVALID TARGETACTORKEY TYPE!"));
        return;
    }

    AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
    CHECK_NULLPTR_RET(TargetActor, LogBehaviorTree, "BTService_GetActorLocation:: REFERENCE OBJECT NOT AN ACTOR!");

    AIController->SetVectorKeyValue(TargetLocationKey.SelectedKeyName, TargetActor->GetActorLocation());    
}

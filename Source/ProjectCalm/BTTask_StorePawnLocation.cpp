// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StorePawnLocation.h"
#include "PhotoSubjectAIController.h"

EBTNodeResult::Type UBTTask_StorePawnLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    if (AIController == nullptr)
    {
        UE_LOG(LogTemp, Error, TEXT("BTTask::StorePawnLocation:: AICONTROLLER NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    APawn* Pawn = AIController->GetPawn();
    if (Pawn == nullptr) 
    {
        UE_LOG(LogTemp, Error, TEXT("BTTask::StorePawnLocation:: PAWN NOT FOUND!"));
        return EBTNodeResult::Failed;
    }

    if(APhotoSubjectAIController* SubjectController = Cast<APhotoSubjectAIController>(AIController))
    {
        SubjectController->SetHomeLocation(Pawn->GetActorLocation());
        return EBTNodeResult::Succeeded;
    }

    UE_LOG(LogTemp, Error, TEXT("BTTask::StorePawnLocation:: COULD NOT CAST AICONTROLLER TO PHOTOSUBJECTAICONTROLLER!"));
    return EBTNodeResult::Failed;
}

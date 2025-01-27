// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StorePawnLocation.h"
#include "PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"


EBTNodeResult::Type UBTTask_StorePawnLocation::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    AAIController* AIController = OwnerComp.GetAIOwner();
    CHECK_NULLPTR_RETVAL(AIController, LogBehaviorTree, "BTTask::LookAt:: AI CONTROLLER NOT FOUND!", EBTNodeResult::Failed);

    APawn* Pawn = AIController->GetPawn();
    CHECK_NULLPTR_RETVAL(Pawn, LogBehaviorTree, "BTTask::LookAt:: PAWN NOT FOUND!", EBTNodeResult::Failed);

    APhotoSubjectAIController* SubjectController = Cast<APhotoSubjectAIController>(AIController);
    CHECK_NULLPTR_RETVAL(SubjectController, LogBehaviorTree, "BTTask::LookAt:: COULD NOT CAST AICONTROLLER TO PHOTOSUBJECTAICONTROLLER!", EBTNodeResult::Failed);

    SubjectController->SetVectorKeyValue(GetSelectedBlackboardKey(), Pawn->GetActorLocation());
    return EBTNodeResult::Succeeded;
}

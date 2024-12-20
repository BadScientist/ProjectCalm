// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetBehavior.h"
#include "PhotoSubjectBehavior.h"
#include "PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTTaskNode_SetBehavior::UBTTaskNode_SetBehavior()
{
    NodeName = TEXT("Set Behavior");

    Behavior = EPhotoSubjectBehavior::NONE;
}

EBTNodeResult::Type UBTTaskNode_SetBehavior::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    CHECK_NULLPTR_RETVAL(AIController, LogBehaviorTree, "BTTask_SetBehavior:: AI CONTROLLER NOT FOUND!", EBTNodeResult::Failed);
    AIController->SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, Behavior);
    
    return EBTNodeResult::Succeeded;
}

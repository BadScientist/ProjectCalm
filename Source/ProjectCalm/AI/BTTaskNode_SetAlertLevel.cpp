// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SetAlertLevel.h"
#include "PhotoSubjectAIController.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTaskNode_SetAlertLevel::UBTTaskNode_SetAlertLevel()
{
    NodeName = TEXT("Set Alert Level");
}

EBTNodeResult::Type UBTTaskNode_SetAlertLevel::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    if (APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner()))
    {
        AIController->SetAlertLevelKeyValue(BBKEY_ALERT_LEVEL, AlertLevel);
        return EBTNodeResult::Succeeded;
    }

    return EBTNodeResult::Failed;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ClearBlackboardKey.h"
#include "BehaviorTree/BlackboardComponent.h"


UBTTask_ClearBlackboardKey::UBTTask_ClearBlackboardKey()
{
    NodeName = TEXT("Clear Blackboard Key");
}

EBTNodeResult::Type UBTTask_ClearBlackboardKey::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    OwnerComp.GetBlackboardComponent()->ClearValue(GetSelectedBlackboardKey());

    return EBTNodeResult::Succeeded;
}

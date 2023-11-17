// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SelectBehavior.h"
#include "WeightedBehavior.h"
#include "PhotoSubjectAIController.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTTaskNode_SelectBehavior::UBTTaskNode_SelectBehavior()
{
    NodeName = TEXT("Select Behavior");
}

EBTNodeResult::Type UBTTaskNode_SelectBehavior::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    if (AIController == nullptr)
    {
        UE_LOG(LogBehaviorTree, Error, TEXT("BTTask::SelectBehavior:: AI CONTROLLER NOT FOUND!"));
        return Result;
    }

    float TotalWeight{0};
    for (FWeightedBehavior Behavior : Behaviors) {TotalWeight += Behavior.GetAdjustedWeight();}

    float Selector = FMath::RandRange(0.0f, TotalWeight);
    float CurrentWeight {0};
    for (FWeightedBehavior Behavior : Behaviors)
    {
        float NextWeight = CurrentWeight + Behavior.GetAdjustedWeight();

        if (CurrentWeight < Selector && Selector <= NextWeight)
        {
            AIController->SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, Behavior.Behavior);
            Behavior.ConsecutiveSelections++;
            Result = EBTNodeResult::Succeeded;
        }
        else {Behavior.ConsecutiveSelections = 0;}

        CurrentWeight = NextWeight;
    }

    return Result;
}

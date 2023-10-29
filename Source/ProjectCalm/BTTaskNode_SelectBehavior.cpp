// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SelectBehavior.h"
#include "PhotoSubjectBehavior.h"
#include "PhotoSubjectAIController.h"

#include "BehaviorTree/BlackboardComponent.h"


UBTTaskNode_SelectBehavior::UBTTaskNode_SelectBehavior()
{
    NodeName = TEXT("Select Behavior");
}

EBTNodeResult::Type UBTTaskNode_SelectBehavior::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    float TotalWeight{0};
    for (FWeightedBehavior Behavior : Behaviors) {TotalWeight += Behavior.Weight;}

    float Selector = FMath::RandRange(0.0f, TotalWeight);
    float CurrentWeight {0};
    for (FWeightedBehavior Behavior : Behaviors)
    {
        float NextWeight = CurrentWeight + Behavior.Weight;
        if (CurrentWeight < Selector && Selector <= NextWeight)
        {
            if(APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner()))
            {
                AIController->SetActiveBehavior(Behavior.Behavior);
            }
            break;
        }
        
        CurrentWeight = NextWeight;
    }

    return EBTNodeResult::Succeeded;
}

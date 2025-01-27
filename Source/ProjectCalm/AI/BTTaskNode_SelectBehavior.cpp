// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_SelectBehavior.h"
#include "WeightedBehavior.h"
#include "PhotoSubjectAIController.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "BehaviorTree/BlackboardComponent.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


UBTTaskNode_SelectBehavior::UBTTaskNode_SelectBehavior()
{
    NodeName = TEXT("Select Behavior");
}

EBTNodeResult::Type UBTTaskNode_SelectBehavior::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    EBTNodeResult::Type Result = EBTNodeResult::Failed;

    APhotoSubjectAIController* AIController = Cast<APhotoSubjectAIController>(OwnerComp.GetAIOwner());
    CHECK_NULLPTR_RETVAL(AIController, LogBehaviorTree, "BTTask_SelectBehavior:: AI CONTROLLER NOT FOUND!", Result);

    float TotalWeight{0};
    for (FWeightedBehavior Behavior : Behaviors) {TotalWeight += Behavior.GetAdjustedWeight();}

    float Selector = FMath::RandRange(0.0f, TotalWeight);
    float CurrentWeight {0};

#ifdef LOCAL_DEBUG_LOGS
    FString BehaviorsString;
#endif

    for (FWeightedBehavior Behavior : Behaviors)
    {

#ifdef LOCAL_DEBUG_LOGS
        BehaviorsString.Append(PCPhotoSubjectBehavior::EnumToString(Behavior.Behavior));
#endif

        float NextWeight = CurrentWeight + Behavior.GetAdjustedWeight();

        if (CurrentWeight < Selector && Selector <= NextWeight)
        {
#ifdef LOCAL_DEBUG_LOGS
            BehaviorsString.Append("*");
#endif
            AIController->SetBehaviorKeyValue(BBKEY_ACTIVE_BEHAVIOR, Behavior.Behavior);
            Behavior.ConsecutiveSelections++;
            Result = EBTNodeResult::Succeeded;
        }
        else {Behavior.ConsecutiveSelections = 0;}

#ifdef LOCAL_DEBUG_LOGS
            BehaviorsString.Append(", ");
#endif

        CurrentWeight = NextWeight;
    }

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogBehaviorTree, Display, TEXT("BTTask_SelectBehavior:: Behaviors: %s"), *BehaviorsString);
#endif

    return Result;
}

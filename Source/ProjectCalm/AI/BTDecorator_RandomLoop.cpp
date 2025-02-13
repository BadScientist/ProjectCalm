// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RandomLoop.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Engine/World.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_RandomLoop)

void UBTDecorator_RandomLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
    UWorld* World = GetWorld();
	float CurrentTime = World == nullptr ? 0.0f : World->GetTimeSeconds();
	FBTRandomLoopDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTRandomLoopDecoratorMemory>(SearchData);
	CHECK_NULLPTR_RET(DecoratorMemory, LogBehaviorTree, "BTDecorator_RandomLoop:: Could not get Decorator Memory!");

	FBTCompositeMemory* ParentMemory = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
	CHECK_NULLPTR_RET(ParentMemory, LogBehaviorTree, "BTDecorator_RandomLoop:: Could not get Parent Memory!");

	const bool bIsSpecialNode = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());

	if ((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
	{
		// initialize counter if it's first activation
		DecoratorMemory->RemainingExecutions = FMath::Max(1, NumLoops.GetValue(SearchData.OwnerComp) + FMath::RandRange(-NumLoopsDeviation, NumLoopsDeviation));
        if (World != nullptr) {DecoratorMemory->TimeStarted = CurrentTime;}
        DecoratorMemory->EndTime = DecoratorMemory->TimeStarted + FMath::RandRange(
			InfiniteLoopTimeoutTime.GetValue(SearchData.OwnerComp) - TimeoutTimeDeviation,
			InfiniteLoopTimeoutTime.GetValue(SearchData.OwnerComp) + TimeoutTimeDeviation);
	}

	bool bShouldLoop = false;
	if (bInfiniteLoop)
	{
		// protect from truly infinite loop within single search
		if (SearchData.SearchId != DecoratorMemory->SearchId)
		{
			if ((InfiniteLoopTimeoutTime.GetValue(SearchData.OwnerComp) < 0.f) || (World != nullptr && DecoratorMemory->EndTime > CurrentTime))
			{
				bShouldLoop = true;
			}
		}

		DecoratorMemory->SearchId = SearchData.SearchId;
	}
	else
	{
		if (DecoratorMemory->RemainingExecutions > 0)
		{
			DecoratorMemory->RemainingExecutions--;
		}
		bShouldLoop = DecoratorMemory->RemainingExecutions > 0;
	}


	// set child selection overrides
	if (bShouldLoop)
	{
		UBTCompositeNode* ParentCompositeNode = GetParentNode();
		if (ParentCompositeNode != nullptr) {ParentCompositeNode->SetChildOverride(SearchData, ChildIndex);}
	}
}

uint16 UBTDecorator_RandomLoop::GetInstanceMemorySize() const
{
	return sizeof(FBTRandomLoopDecoratorMemory);
}

void UBTDecorator_RandomLoop::DescribeRuntimeValues(const UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString> &Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	CHECK_NULLPTR_RET(NodeMemory, LogBehaviorTree, "BTDecorator_RandomLoop:: Null Node Memory passed to DescribeRuntimeValues!");
	FBTRandomLoopDecoratorMemory* DecoratorMemory = (FBTRandomLoopDecoratorMemory*)NodeMemory;

	if (!bInfiniteLoop)
	{
		Values.Add(FString::Printf(TEXT("loops remaining: %d"), DecoratorMemory->RemainingExecutions));
	}
	else if (InfiniteLoopTimeoutTime.GetValue(OwnerComp) > 0.f)
	{
		UWorld* World = GetWorld();
		float CurrentTime = World == nullptr ? DecoratorMemory->EndTime : World->GetTimeSeconds();
		const float TimeRemaining = FMath::Max(DecoratorMemory->EndTime - CurrentTime, 0.f);
		Values.Add(FString::Printf(TEXT("time remaining: %s"), *FString::SanitizeFloat(TimeRemaining)));
	}
}

FString UBTDecorator_RandomLoop::GetStaticDescription() const
{
	// basic info: infinite / num loops
	if (bInfiniteLoop)
	{
		if (InfiniteLoopTimeoutTime.GetKey().IsNone() && InfiniteLoopTimeoutTime.GetValue(static_cast<const UBehaviorTreeComponent*>(nullptr)) <= 0.f)
		{
			return FString::Printf(TEXT("%s: infinite"), *UBTDecorator::GetStaticDescription());
		}
		else
		{
			return FString::Printf(
                TEXT("%s: loop for %s - %s seconds"),
                *UBTDecorator::GetStaticDescription(),
                *FString::SanitizeFloat(InfiniteLoopTimeoutTime.GetValue(static_cast<const UBehaviorTreeComponent*>(nullptr)) - TimeoutTimeDeviation),
                *FString::SanitizeFloat(InfiniteLoopTimeoutTime.GetValue(static_cast<const UBehaviorTreeComponent*>(nullptr)) + TimeoutTimeDeviation));
		}
	}
	else
	{
		return FString::Printf(
            TEXT("%s: %d - %d loops"),
            *UBTDecorator::GetStaticDescription(),
            NumLoops.GetValue(static_cast<const UBehaviorTreeComponent*>(nullptr)) - NumLoopsDeviation,
            NumLoops.GetValue(static_cast<const UBehaviorTreeComponent*>(nullptr)) + NumLoopsDeviation);
	}
}

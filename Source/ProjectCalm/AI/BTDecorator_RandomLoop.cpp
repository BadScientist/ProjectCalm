// Fill out your copyright notice in the Description page of Project Settings.


#include "BTDecorator_RandomLoop.h"

#include "Engine/World.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "BehaviorTree/Composites/BTComposite_SimpleParallel.h"


#include UE_INLINE_GENERATED_CPP_BY_NAME(BTDecorator_RandomLoop)

void UBTDecorator_RandomLoop::OnNodeActivation(FBehaviorTreeSearchData& SearchData)
{
	FBTRandomLoopDecoratorMemory* DecoratorMemory = GetNodeMemory<FBTRandomLoopDecoratorMemory>(SearchData);
	FBTCompositeMemory* ParentMemory = GetParentNode()->GetNodeMemory<FBTCompositeMemory>(SearchData);
	const bool bIsSpecialNode = GetParentNode()->IsA(UBTComposite_SimpleParallel::StaticClass());

	if ((bIsSpecialNode && ParentMemory->CurrentChild == BTSpecialChild::NotInitialized) ||
		(!bIsSpecialNode && ParentMemory->CurrentChild != ChildIndex))
	{
		// initialize counter if it's first activation
		DecoratorMemory->RemainingExecutions = FMath::Max(1, NumLoops + FMath::RandRange(-NumLoopsDeviation, NumLoopsDeviation));
        DecoratorMemory->TimeStarted = GetWorld()->GetTimeSeconds();
        DecoratorMemory->EndTime = DecoratorMemory->TimeStarted + FMath::RandRange(InfiniteLoopTimeoutTime - TimeoutTimeDeviation, InfiniteLoopTimeoutTime + TimeoutTimeDeviation);
	}

	bool bShouldLoop = false;
	if (bInfiniteLoop)
	{
		// protect from truly infinite loop within single search
		if (SearchData.SearchId != DecoratorMemory->SearchId)
		{
			if ((InfiniteLoopTimeoutTime < 0.f) || (DecoratorMemory->EndTime > GetWorld()->GetTimeSeconds()))
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
		GetParentNode()->SetChildOverride(SearchData, ChildIndex);
	}
}

uint16 UBTDecorator_RandomLoop::GetInstanceMemorySize() const
{
	return sizeof(FBTRandomLoopDecoratorMemory);
}

void UBTDecorator_RandomLoop::DescribeRuntimeValues(const UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString> &Values) const
{
	Super::DescribeRuntimeValues(OwnerComp, NodeMemory, Verbosity, Values);

	if (!bInfiniteLoop)
	{
		FBTRandomLoopDecoratorMemory* DecoratorMemory = (FBTRandomLoopDecoratorMemory*)NodeMemory;
		Values.Add(FString::Printf(TEXT("loops remaining: %d"), DecoratorMemory->RemainingExecutions));
	}
	else if (InfiniteLoopTimeoutTime > 0.f)
	{
		FBTRandomLoopDecoratorMemory* DecoratorMemory = (FBTRandomLoopDecoratorMemory*)NodeMemory;

		const float TimeRemaining = FMath::Max(DecoratorMemory->EndTime - GetWorld()->GetTimeSeconds(), 0.f);
		Values.Add(FString::Printf(TEXT("time remaining: %s"), *FString::SanitizeFloat(TimeRemaining)));
	}
}

FString UBTDecorator_RandomLoop::GetStaticDescription() const
{
	// basic info: infinite / num loops
	if (bInfiniteLoop)
	{
		if (InfiniteLoopTimeoutTime < 0.f)
		{
			return FString::Printf(TEXT("%s: infinite"), *Super::GetStaticDescription());
		}
		else
		{
			return FString::Printf(
                TEXT("%s: loop for %s - %s seconds"),
                *Super::GetStaticDescription(),
                *FString::SanitizeFloat(InfiniteLoopTimeoutTime - TimeoutTimeDeviation),
                *FString::SanitizeFloat(InfiniteLoopTimeoutTime + TimeoutTimeDeviation));
		}
	}
	else
	{
		return FString::Printf(
            TEXT("%s: %d - %d loops"),
            *Super::GetStaticDescription(),
            NumLoops - NumLoopsDeviation,
            NumLoops + NumLoopsDeviation);
	}
}


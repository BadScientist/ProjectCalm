// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_Loop.h"
#include "BTDecorator_RandomLoop.generated.h"

struct FBTRandomLoopDecoratorMemory : FBTLoopDecoratorMemory
{
	float EndTime;
};

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTDecorator_RandomLoop : public UBTDecorator_Loop
{
	GENERATED_BODY()

	/** number of executions */
	UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="!bInfiniteLoop", ClampMin="0"))
	int32 NumLoopsDeviation;

	/** timeout (when looping infinitely, when we finish a loop we will check whether we have spent this time looping, if we have we will stop looping). A negative value means loop forever. */
	UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "bInfiniteLoop", ClampMin="0.0"))
	float TimeoutTimeDeviation;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
};

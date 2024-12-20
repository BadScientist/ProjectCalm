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

	UPROPERTY(Category=Decorator, EditAnywhere, meta=(EditCondition="!bInfiniteLoop", ClampMin="0"))
	int32 NumLoopsDeviation;

	UPROPERTY(Category = Decorator, EditAnywhere, meta = (EditCondition = "bInfiniteLoop", ClampMin="0.0"))
	float TimeoutTimeDeviation;

	virtual uint16 GetInstanceMemorySize() const override;
	virtual void DescribeRuntimeValues(const UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTDescriptionVerbosity::Type Verbosity, TArray<FString>& Values) const override;
	virtual FString GetStaticDescription() const override;

protected:
	virtual void OnNodeActivation(FBehaviorTreeSearchData& SearchData) override;
	
};

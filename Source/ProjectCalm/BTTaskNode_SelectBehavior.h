// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SelectBehavior.generated.h"

struct FWeightedBehavior;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTaskNode_SelectBehavior : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SelectBehavior();

private:
	/** List of Behaviors the pawn can perform. */
	UPROPERTY(Category = Behavior, EditAnywhere)
	TArray<FWeightedBehavior> Behaviors;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

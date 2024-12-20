// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_SetBehavior.generated.h"

enum EPhotoSubjectBehavior;


/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTaskNode_SetBehavior : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SetBehavior();

private:
	UPROPERTY(Category = Behavior, EditAnywhere)
	TEnumAsByte<EPhotoSubjectBehavior> Behavior;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

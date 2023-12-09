// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"

#include "AlertLevel.h"
#include "BTTaskNode_SetAlertLevel.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTaskNode_SetAlertLevel : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTaskNode_SetAlertLevel();

private:
	/** The max distance the pawn can wander from its current location in cm. */
	UPROPERTY(Category = AlertLevel, EditAnywhere)
	TEnumAsByte<EAlertLevel> AlertLevel{EAlertLevel::CALM};

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};

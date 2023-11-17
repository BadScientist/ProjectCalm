// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_MoveToAtSpeed.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_MoveToAtSpeed : public UBTTask_MoveTo
{
	GENERATED_BODY()

public:
	UBTTask_MoveToAtSpeed();

private:
	UPROPERTY(config, Category = Node, EditAnywhere)
	TEnumAsByte<EMovementMode> MovementMode{EMovementMode::MOVE_Walking};

	/** In cm/s */
	UPROPERTY(config, Category = Node, EditAnywhere, meta=(ClampMin = "1.0", UIMin="1.0"))
	float MovementSpeed{1.0f};

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void SetMovementModeAndSpeed(UBehaviorTreeComponent &OwnerComp);
	
};

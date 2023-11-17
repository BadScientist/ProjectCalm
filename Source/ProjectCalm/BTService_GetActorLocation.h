// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "BTService_GetActorLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTService_GetActorLocation : public UBTService
{
	GENERATED_BODY()
	
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

protected:
	/** Actor key storing the Actor whose location is to be stored. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector TargetActorKey;

	/** FVector key for storing the Actor's location. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector TargetLocationKey;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

private:
	void StoreTargetActorLocation(UBehaviorTreeComponent &OwnerComp);
	
};

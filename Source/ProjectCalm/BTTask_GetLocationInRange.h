// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetLocationInRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_GetLocationInRange : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetLocationInRange();

private:
	/** The max distance the pawn can wander from its current location in cm. */
	UPROPERTY(Category = Range, EditAnywhere, meta = (ClampMin = "1.0", UIMin = "1.0"))
	float MaxDistanceFromPawn{1000};
	
	/** If 0, the pawn will be allowed to move an infinite distance from its Home location. */
	UPROPERTY(Category = Range, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxDistanceFromHome{10000};

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	FVector FindRandomLocationInRange(FVector HomeLocation, FVector CurrentLocation);
	bool ProjectLocationToSurface(FVector InLocation, FVector& OutLocation);
	float CalculateMaxDistance(float AngleOfLocation, float DistanceFromHome);

protected:
	/** FVector key defining the center of the static region in which the pawn may wander. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector HomeLocationKey;

	/** FVector Key for storing the output location*/
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector DestinationKey;
};

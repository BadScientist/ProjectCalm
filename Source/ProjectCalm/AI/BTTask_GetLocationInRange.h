// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_GetLocation.h"
#include "BTTask_GetLocationInRange.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_GetLocationInRange : public UBTTask_GetLocation
{
	GENERATED_BODY()

public:
	UBTTask_GetLocationInRange();

protected:
	/** FVector key defining the center of the static region in which the pawn may wander. */
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector HomeLocationKey;

private:
	/** Max distance in cm from the given Home Location. If 0, the distance will not be constrained. */
	UPROPERTY(Category = Range, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MaxDistanceFromHome{10000};

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	FVector FindRandomLocationInRange(FVector HomeLocation, FVector CurrentLocation);
	float CalculateMaxDistance(float AngleOfLocation, float DistanceFromHome);
	FVector CalculateDirectionFromHome(FVector HomeLocation, FVector CurrentLocation);

};

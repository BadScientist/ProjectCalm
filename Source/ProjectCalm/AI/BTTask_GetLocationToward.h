// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BTTask_GetLocation.h"
#include "BTTask_GetLocationToward.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_GetLocationToward : public UBTTask_GetLocation
{
	GENERATED_BODY()

public:
	UBTTask_GetLocationToward();

private:
	/** Min distance in cm from the pawn's current location (takes priority over max). */
	UPROPERTY(Category = Range, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float MinDistanceFromPawn{2500};

	/** Get a location in a direction away from the reference. */
	UPROPERTY(Category = Direction, EditAnywhere)
	bool bReverseDirection{false};

	/** Allow the distance to the new location to be greater than the distance to the reference. */
	UPROPERTY(Category = Direction, EditAnywhere)
	bool bAllowMovePastReference{false};

	/** Angle in degrees from the direction of the reference in which the returned location must fall. */
	UPROPERTY(Category = Direction, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "180.0", UIMax = "180.0"))
	float Tolerance{0.0f};

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	/** Actor or FVector Location to move toward.*/
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector ReferenceKey;
	
	bool GetReferenceLocation(UBlackboardComponent *BlackboardComponent, FVector& OutLocation);
	FVector CalculateReferenceDirection(FVector PawnLocation, FVector ReferenceLocation);
	
};

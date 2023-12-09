// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetLocation.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UBTTask_GetLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_GetLocation(){};

protected:
	/** FVector Key for storing the output location*/
	UPROPERTY(EditAnywhere, Category=Blackboard)
	struct FBlackboardKeySelector DestinationKey;

	/** Max distance in cm from the pawn's current location. */
	UPROPERTY(Category = Range, EditAnywhere, meta = (ClampMin = "1.0", UIMin = "1.0"))
	float MaxDistanceFromPawn{1000};
	
	/** Ensure the location returned is on a Nav Mesh. */
	UPROPERTY(Category = Elevation, EditAnywhere)
	bool bFindPointOnGround{true};
	
	/** Max distance above the ground in cm. */
	UPROPERTY(Category = Elevation, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition="!bFindPointOnGround"))
	float MaxDistanceFromGround{2500};
	
	/** Min distance above the ground in cm. */
	UPROPERTY(Category = Elevation, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition="!bFindPointOnGround"))
	float MinDistanceFromGround{200};
	
	/** Max distance above or below the input location in cm. */
	UPROPERTY(Category = Elevation, EditAnywhere, meta = (ClampMin = "0.0", UIMin = "0.0", EditCondition="!bFindPointOnGround"))
	float MaxElevationChange{1000};
    
    float CalculateVectorDistance2D(FVector VectorA, FVector VectorB);
    bool FindGround(FVector InLocation, FHitResult& OutHit);
    FVector TranslateToRandomElevation(FVector InLocation);
	bool ProjectLocationToSurface(FVector InLocation, FVector& OutLocation);

};

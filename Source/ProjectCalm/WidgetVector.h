#pragma once

#include "WidgetVector.generated.h"

USTRUCT(BlueprintType)
struct FWidgetVector {
	GENERATED_BODY()

	FWidgetVector(){};
	FWidgetVector(FVector InVector) {Vector = InVector;}
	FWidgetVector(float X, float Y, float Z) {Vector = FVector(X, Y, Z);}

	static FWidgetVector Zero() {return FWidgetVector(0, 0, 0);}
		
	UPROPERTY(EditAnywhere, Meta = (MakeEditWidget = true))
	FVector Vector;
};
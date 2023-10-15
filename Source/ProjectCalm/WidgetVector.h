#pragma once

#include "WidgetVector.generated.h"


USTRUCT(BlueprintType)
struct FWidgetVector {
	GENERATED_BODY()

	FWidgetVector(){Vector = FVector::ZeroVector;};
	FWidgetVector(FVector InVector) : Vector(InVector){};
	FWidgetVector(float X, float Y, float Z) : Vector(FVector(X, Y, Z)){};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (MakeEditWidget = true))
	FVector Vector;	

	static const FWidgetVector Zero() {return FWidgetVector(0, 0, 0);};
};
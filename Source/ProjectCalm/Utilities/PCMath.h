#pragma once

#include "CoreMinimal.h"


class PCMath
{
public:
    /** Returns the angle from -180 to 180 degrees from the Actor's forward vector to the direction of the given location. */
    static float CalculateLocationAngle2D(AActor* Actor, FVector Location)
    {
        FVector ActorForward = Actor->GetActorForwardVector();
        FVector Direction = (Location - Actor->GetActorLocation()).GetSafeNormal2D();
        float UnsignedAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(ActorForward, Direction)));

        FVector ActorRight = Actor->GetActorRightVector();
        float Sign = FVector::DotProduct(ActorRight, Direction);
        float SignedAngle = UnsignedAngle;
        if (Sign < 0) {SignedAngle *= -1;}

        return SignedAngle;
    }

};
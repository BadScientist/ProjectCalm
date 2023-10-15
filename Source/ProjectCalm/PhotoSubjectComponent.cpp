#include "PhotoSubjectComponent.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"


#define MAX_TRACE_LENGTH 3000
#define DESPAWN_DISTANCE 10000


UPhotoSubjectComponent::UPhotoSubjectComponent()
{
}

bool UPhotoSubjectComponent::Spawn()
{
    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Finding ground"));
    FVector TraceStart = GetOwner()->GetActorLocation();
    FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - MAX_TRACE_LENGTH);
    FHitResult OutHit;

    bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic);
    if (Hit)
    {
        GetOwner()->SetActorLocation(OutHit.Location);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Failed to find point on ground"));
    return false;
}

bool UPhotoSubjectComponent::Despawn(FVector PlayerLocation, FVector PlayerForwardVector)
{
    FVector SubjectLocation = GetComponentLocation();
    double DistanceToPlayer = FVector::Distance(SubjectLocation, PlayerLocation);
    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Distance to player: %f"), DistanceToPlayer);
    if (DistanceToPlayer < DESPAWN_DISTANCE) {return false;}
    
    // TODO: Fix hardcoded values
    FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(PlayerLocation, SubjectLocation);
    double Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Direction, PlayerForwardVector)));
    bool bInVisionCone = Angle <= 45;
    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Angle from player: %f"), Angle);
    
    FHitResult OutHit;    
    FVector TraceStart = PlayerLocation + Direction * 55;
    bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, SubjectLocation, ECollisionChannel::ECC_Visibility);
    bool bInLineOfSight = Hit && OutHit.GetActor() == GetOwner();
    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Visible to player: %s"), bInLineOfSight ? *FString("yes") : *FString("no"));
    if (bInVisionCone && bInLineOfSight) {return false;}

    return GetOwner()->Destroy();
}

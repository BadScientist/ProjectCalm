#include "PhotoSubjectComponent.h"
#include "PhotoSubjectAIController.h"

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
    FVector TraceStart = GetOwner()->GetActorLocation();
    FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - MAX_TRACE_LENGTH);
    FHitResult OutHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());

    bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic, Params);
    if (Hit)
    {
        GetOwner()->SetActorLocation(OutHit.Location);
        return true;
    }

    UE_LOG(LogTemp, Warning, TEXT("PhotoSubjectComponent:: Failed to find point on ground"));
    return false;
}

bool UPhotoSubjectComponent::Despawn(AActor* Player)
{
    FVector SubjectLocation = GetComponentLocation();
    double DistanceToPlayer = FVector::Distance(SubjectLocation, Player->GetActorLocation());
    if (DistanceToPlayer < DESPAWN_DISTANCE) {return false;}
    
    // TODO: Fix hardcoded values
    FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(Player->GetActorLocation(), SubjectLocation);
    double Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Direction, Player->GetActorForwardVector())));
    bool bInVisionCone = Angle <= 45;
    
    FHitResult OutHit;
    FCollisionQueryParams Params;
    TArray<AActor*> AttachedActors;
    Player->GetAttachedActors(AttachedActors, true, true);
    Params.AddIgnoredActor(Player);
    Params.AddIgnoredActors(AttachedActors);
    bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, Player->GetActorLocation(), SubjectLocation, ECollisionChannel::ECC_Visibility);
    bool bInLineOfSight = Hit && OutHit.GetActor() == GetOwner();
    if (bInVisionCone && bInLineOfSight) {return false;}

    return GetOwner()->Destroy();
}

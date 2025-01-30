#include "PhotoSubjectComponent.h"
#include "ProjectCalm/AI/PhotoSubjectAIController.h"

#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Character.h"


#define MAX_TRACE_LENGTH 30000
#define DESPAWN_DISTANCE 10000


UPhotoSubjectComponent::UPhotoSubjectComponent()
{
}

bool UPhotoSubjectComponent::Spawn(float RegionHeight)
{
    FVector TraceStart = GetOwner()->GetActorLocation();
    FVector TraceEnd = FVector(TraceStart.X, TraceStart.Y, TraceStart.Z - (RegionHeight <=0 ? MAX_TRACE_LENGTH : RegionHeight));
    FHitResult OutHit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(GetOwner());
    Params.bReturnPhysicalMaterial = true;

    bool Hit = GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic, Params);
    if (Hit && OutHit.PhysMaterial.IsValid() && ValidSurfaces.Contains(OutHit.PhysMaterial->SurfaceType))
    {
        GetOwner()->SetActorLocation(OutHit.Location);
        return true;
    }

    return false;
}

bool UPhotoSubjectComponent::Despawn(AActor* Player)
{
    FVector SubjectLocation = GetComponentLocation();
    double DistanceToPlayer = FVector::Distance(SubjectLocation, Player->GetActorLocation());
    if (DistanceToPlayer < DESPAWN_DISTANCE) {return false;}
    
    // @todo: Fix hardcoded values
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

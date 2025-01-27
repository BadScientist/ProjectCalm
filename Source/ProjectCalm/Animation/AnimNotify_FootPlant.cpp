// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FootPlant.h"
#include "ProjectCalm/Gameplay/NoiseMakerComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

#ifdef PC_DEBUG_DRAW_SHAPES
    // #define LOCAL_DEBUG_SHAPES
#endif


void UAnimNotify_FootPlant::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    if (MeshComp != nullptr) {Owner = MeshComp->GetOwner();}
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");

    if (!IsCharacterMoving()) {return;}

    CalculateSpeedRatio();
    
    PrepareFootstepSound();

    Super::Notify(MeshComp, Animation, EventReference);
}

void UAnimNotify_FootPlant::ReportNoiseEvent()
{
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");
    if (FMath::IsNearlyZero(BaseNoiseLevel)) {return;}

    float NoiseLevel = FMath::RandRange(FMath::Max(BaseNoiseLevel - NoiseDeviation, 0.0f), BaseNoiseLevel + NoiseDeviation);
    float FinalNoiseLevel = FMath::Clamp(NoiseLevel + 30.0f * log10f(SpeedRatio), 0.0f, NoiseLevel + 20);

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogAnimation, Display, TEXT("AnimNotify_FootPlant:: ReportNoiseEvent: Strength: %f"), FinalNoiseLevel);
#endif

    UAISense_Hearing::ReportNoiseEvent(Owner, Owner->GetActorLocation(), FinalNoiseLevel, Owner, MAX_NOISE_RANGE);
}

void UAnimNotify_FootPlant::PrepareFootstepSound()
{
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");
    SoundLocation = Owner->GetActorLocation() + FootOffset;

    FVector TraceStart = SoundLocation + FVector(0.0f, 0.0f, 50.0f);
    FVector TraceEnd = SoundLocation - FVector(0.0f, 0.0f, 200.0f);
    FCollisionQueryParams Params;
    Params.bReturnPhysicalMaterial = true;
    Params.AddIgnoredActor(Owner);

#ifdef LOCAL_DEBUG_SHAPES
    DrawDebugLine(Owner->GetWorld(), TraceStart, TraceEnd, FColor::Red, true, 100.0f, ESceneDepthPriorityGroup::SDPG_Foreground, 10.0);
#endif

    FHitResult OutHit;
    bool bHit = Owner->GetWorld()->LineTraceSingleByChannel(OutHit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, Params);
    
    EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;
    if (bHit)
    {
        if (OutHit.PhysMaterial.IsValid())
        {
            SurfaceType = OutHit.PhysMaterial->SurfaceType;

#ifdef LOCAL_DEBUG_LOGS
            UE_LOG(LogAnimation, Warning, TEXT("AnimNotify_FootPlant:: PhysMatIsValid: %i, SurfaceType: %s"), OutHit.PhysMaterial.IsValid(),
                (SurfaceType == EPhysicalSurface::SurfaceType1 ? *FString("SurfaceType1") :
                SurfaceType == EPhysicalSurface::SurfaceType2 ? *FString("SurfaceType2") :
                SurfaceType == EPhysicalSurface::SurfaceType3 ? *FString("SurfaceType3") :
                SurfaceType == EPhysicalSurface::SurfaceType4 ? *FString("SurfaceType4") :
                SurfaceType == EPhysicalSurface::SurfaceType_Default ? *FString("SurfaceType_Default") :
                *FString("Error")));
#endif
        }
    }

#ifdef LOCAL_DEBUG_LOGS
    else {UE_LOG(LogAnimation, Warning, TEXT("AnimNotify_FootPlant:: Failed to find Terrain between %s and %s!"), *(TraceStart.ToCompactString()), *(TraceEnd.ToCompactString()));}    
#endif
    
    FName* MapValue = SoundMap.Find(SurfaceType);
    CHECK_NULLPTR_RET(MapValue, LogAnimation, "AnimNotify_FootPlant:: No valid Surface Type!");
    if (MapValue != nullptr)
    {
        SoundName = *MapValue;
        VolumeMultiplier = FMath::Clamp(SpeedRatio, 0.65f, 1.35f);
    }
}

bool UAnimNotify_FootPlant::IsCharacterMoving()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);

    CHECK_NULLPTR_RETVAL(OwnerCharacter, LogAnimation, "AnimNotify_FootPlant:: No Owning Character!", false);

    return !FMath::IsNearlyZero(OwnerCharacter->GetVelocity().Length());
}

void UAnimNotify_FootPlant::CalculateSpeedRatio()
{
    ACharacter* OwnerCharacter = Cast<ACharacter>(Owner);
    UCharacterMovementComponent* MovementComp = (OwnerCharacter == nullptr) ? nullptr :
        Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
    
    SpeedRatio = MovementComp == nullptr ? 1.0f : MovementComp->Velocity.Size() / AnimBlendSpaceMovementSpeed;
}

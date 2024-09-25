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

#define MAX_NOISE_RANGE 10000.0f


void UAnimNotify_FootPlant::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* Owner{nullptr};
    if (MeshComp != nullptr) {Owner = MeshComp->GetOwner();}
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");

    ReportNoiseEvent(Owner);
    PlayFootstepSound(Owner);
}

void UAnimNotify_FootPlant::ReportNoiseEvent(AActor* OwningActor)
{
    CHECK_NULLPTR_RET(OwningActor, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");
    if (FMath::IsNearlyZero(BaseNoiseLevel)) {return;}
    ACharacter* OwnerCharacter = Cast<ACharacter>(OwningActor);
    UCharacterMovementComponent* MovementComp = (OwnerCharacter == nullptr) ? nullptr :
        Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());

    float NoiseLevel = FMath::RandRange(FMath::Max(BaseNoiseLevel - NoiseDeviation, 0.0f), BaseNoiseLevel + NoiseDeviation);
    if (MovementComp != nullptr) {NoiseLevel *= MovementComp->Velocity.Size() / AnimBlendSpaceMovementSpeed;}

#ifdef PC_DEBUG_LOGS
    UE_LOG(LogAnimation, Display, TEXT("AnimNotify_FootPlant:: ReportNoiseEvent: Strength: %f"), NoiseLevel);
#endif

    UAISense_Hearing::ReportNoiseEvent(OwningActor, OwningActor->GetActorLocation(), NoiseLevel, OwningActor, MAX_NOISE_RANGE);
}

void UAnimNotify_FootPlant::PlayFootstepSound(AActor* OwningActor)
{
    CHECK_NULLPTR_RET(OwningActor, LogAnimation, "AnimNotify_FootPlant:: No owning Actor found!");
    FVector FootLocation = OwningActor->GetActorLocation() + FootOffset;
    FVector TraceEnd = FootLocation - FVector(0.0f, 0.0f, 200.0f);
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(OwningActor);

    FHitResult OutHit;
    if (!(GetWorld()->LineTraceSingleByChannel(OutHit, FootLocation, TraceEnd, ECollisionChannel::ECC_Visibility, Params))){return;}
    
    EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;
    if (OutHit.PhysMaterial.IsValid()) {SurfaceType = OutHit.PhysMaterial->SurfaceType;}
    
    USoundCue** MapValue = SoundMap.Find(SurfaceType);
    if (MapValue != nullptr)
    {
        USoundCue* SoundCue = *MapValue;
        if (SoundCue != nullptr) {UGameplayStatics::PlaySoundAtLocation(OwningActor, SoundCue, FootLocation);}
    }
}

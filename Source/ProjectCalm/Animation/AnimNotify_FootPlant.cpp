// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_FootPlant.h"
#include "ProjectCalm/Gameplay/NoiseMakerComponent.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

void UAnimNotify_FootPlant::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* Owner{nullptr};
    if (MeshComp != nullptr) {Owner = MeshComp->GetOwner();}
    
    if (!FMath::IsNearlyZero(BaseNoiseLevel))
    {

        UNoiseMakerComponent* NoiseMaker{nullptr};
        UCharacterMovementComponent* MovementComp{nullptr};
        if (Owner != nullptr)
        {
            NoiseMaker = Owner->FindComponentByClass<UNoiseMakerComponent>();

            if (ACharacter* OwnerCharacter = Cast<ACharacter>(Owner))
            {
                MovementComp = Cast<UCharacterMovementComponent>(OwnerCharacter->GetMovementComponent());
            }
        }

        float NoiseLevel = FMath::RandRange(FMath::Max(BaseNoiseLevel - NoiseDeviation, 0.0f), BaseNoiseLevel + NoiseDeviation);
        if (MovementComp != nullptr)
        {
            NoiseLevel *= MovementComp->Velocity.Size() / AnimBlendSpaceMovementSpeed;
        }

        if (NoiseMaker != nullptr) {NoiseMaker->MakeMovementNoise(NoiseLevel);}
    }

    if (Owner != nullptr)
    {
        FVector FootLocation = Owner->GetActorLocation() + FootOffset;
        FVector TraceEnd = FootLocation - FVector(0.0f, 0.0f, 200.0f);
        FCollisionQueryParams Params;
        Params.AddIgnoredActor(Owner);

        FHitResult OutHit;
        bool bHit = GetWorld()->LineTraceSingleByChannel(OutHit, FootLocation, TraceEnd, ECollisionChannel::ECC_Visibility, Params);

        if (bHit)
        {
            EPhysicalSurface SurfaceType = EPhysicalSurface::SurfaceType_Default;
            if (OutHit.PhysMaterial.IsValid()) {SurfaceType = OutHit.PhysMaterial->SurfaceType;}
            
            USoundCue** MapValue = SoundMap.Find(SurfaceType);
            if (MapValue != nullptr)
            {
                USoundCue* SoundCue = *MapValue;
                if (SoundCue != nullptr) {UGameplayStatics::PlaySoundAtLocation(Owner, SoundCue, FootLocation);}
            }
        }
    }
}

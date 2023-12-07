// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CameraAnimationEnded.h"
#include "PhotoCameraEquipment.h"

#include "Kismet/GameplayStatics.h"

void UAnimNotify_CameraAnimationEnded::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* OwnerActor = MeshComp->GetOwner();
    if (OwnerActor == nullptr)
    {
        UE_LOG(LogAnimation, Error, TEXT("AnimNotify_CameraAnimationEnded:: No Owning Actor found!"))
        return;
    }

    TArray<AActor*> AttachedActors;
    OwnerActor->GetAttachedActors(AttachedActors);

    for (AActor* AttachedActor : AttachedActors)
    {
        if (APhotoCameraEquipment* PhotoCamera = Cast<APhotoCameraEquipment>(AttachedActor))
        {
            PhotoCamera->OnAnimationEnded();
            return;
        }
    }

    UE_LOG(LogAnimation, Error, TEXT("AnimNotify_CameraAnimationEnded:: No PhotoCamera found!"))
}

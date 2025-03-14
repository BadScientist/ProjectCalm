// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_CameraAnimationEnded.h"
#include "ProjectCalm/Inventory/PhotoCameraEquipment.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Kismet/GameplayStatics.h"

#ifdef PC_DEBUG_LOGS
	#define LOCAL_DEBUG_LOGS
#endif


void UAnimNotify_CameraAnimationEnded::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    #ifdef LOCAL_DEBUG_LOGS
        UE_LOG(LogAnimation, Warning, TEXT("UAnimNotify_CameraAnimationEnded:: Notify called"));
    #endif

    AActor* OwnerActor = MeshComp->GetOwner();
    CHECK_NULLPTR_RET(OwnerActor,LogAnimation , "AnimNotify_CameraAnimationEnded:: No Owning Actor found!");

    TArray<AActor*> AttachedActors;
    OwnerActor->GetAttachedActors(AttachedActors);

    APhotoCameraEquipment* PhotoCamera{nullptr};
    for (AActor* AttachedActor : AttachedActors)
    {
        PhotoCamera = Cast<APhotoCameraEquipment>(AttachedActor);
        if (PhotoCamera != nullptr) {break;}
    }

    CHECK_NULLPTR_RET(PhotoCamera, LogAnimation, "AnimNotify_CameraAnimationEnded:: No PhotoCamera found!");

    PhotoCamera->OnAnimationEnded();
}

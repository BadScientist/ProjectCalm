// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlaySoundCue.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"


void UAnimNotify_PlaySoundCue::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    AActor* Owner{nullptr};
    if (MeshComp != nullptr) {Owner = MeshComp->GetOwner();}
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");

    ReportNoiseEvent(Owner);
    PlaySoundCue(Owner, SoundLocation == FVector::ZeroVector ? Owner->GetActorLocation() : SoundLocation);
}

void UAnimNotify_PlaySoundCue::ReportNoiseEvent(AActor* OwningActor)
{
    CHECK_NULLPTR_RET(OwningActor, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");
    if (FMath::IsNearlyZero(BaseNoiseLevel)) {return;}
    float NoiseLevel = FMath::RandRange(FMath::Max(BaseNoiseLevel - NoiseDeviation, 0.0f), BaseNoiseLevel + NoiseDeviation);

#ifdef PC_DEBUG_LOGS
    UE_LOG(LogAnimation, Display, TEXT("AnimNotify_PlaySoundCue:: ReportNoiseEvent: Strength: %f"), NoiseLevel);
#endif

    UAISense_Hearing::ReportNoiseEvent(OwningActor, OwningActor->GetActorLocation(), NoiseLevel, OwningActor, MAX_NOISE_RANGE);
}

void UAnimNotify_PlaySoundCue::PlaySoundCue(AActor* OwningActor, FVector Location)
{
    CHECK_NULLPTR_RET(OwningActor, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");

    UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(OwningActor);
    CHECK_NULLPTR_RET(GameInstance, LogAnimation, "AnimNotify_PlaySoundCue:: No Game Instance found!");

    GameInstance->PlayDiageticSound(SoundName, OwningActor, Location, VolumeMultiplier);
}

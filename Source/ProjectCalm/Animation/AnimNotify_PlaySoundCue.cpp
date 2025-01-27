// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimNotify_PlaySoundCue.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Perception/AIPerceptionSystem.h"
#include "Perception/AISense_Hearing.h"
#include "Sound/SoundCue.h"
#include "Kismet/GameplayStatics.h"

#ifdef PC_DEBUG_LOGS
    // #define LOCAL_DEBUG_LOGS
#endif


void UAnimNotify_PlaySoundCue::Notify(USkeletalMeshComponent *MeshComp, UAnimSequenceBase *Animation, const FAnimNotifyEventReference &EventReference)
{
    if (MeshComp != nullptr) {Owner = MeshComp->GetOwner();}
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");

    if (!FMath::IsNearlyEqual(ChanceToPlay, 1.0f) && FMath::RandRange(0.0f, 1.0f) > ChanceToPlay) {return;}

    float RandomDelay = FMath::RandRange(FMath::Max(0, Delay - DelayDeviation), Delay + DelayDeviation);

    if (!FMath::IsNearlyZero(RandomDelay) && RandomDelay > 0)
    {
        UWorld* World = Owner->GetWorld();
        CHECK_NULLPTR_RET(World, LogAnimation, "AnimNotify_PlaySoundCue:: Could not get the World!");

        World->GetTimerManager().SetTimer(
            DelayTimerHandle,
            this,
            &UAnimNotify_PlaySoundCue::PlaySoundCue,
            RandomDelay);
        return;
    }

    PlaySoundCue();
}

void UAnimNotify_PlaySoundCue::ReportNoiseEvent()
{
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");
    if (FMath::IsNearlyZero(BaseNoiseLevel)) {return;}
    float NoiseLevel = FMath::RandRange(FMath::Max(BaseNoiseLevel - NoiseDeviation, 0.0f), BaseNoiseLevel + NoiseDeviation);

#ifdef LOCAL_DEBUG_LOGS
    UE_LOG(LogAnimation, Display, TEXT("AnimNotify_PlaySoundCue:: ReportNoiseEvent: Strength: %f"), NoiseLevel);
#endif

    UAISense_Hearing::ReportNoiseEvent(Owner, Owner->GetActorLocation(), NoiseLevel, Owner, MAX_NOISE_RANGE);
}

void UAnimNotify_PlaySoundCue::PlaySoundCue()
{
    CHECK_NULLPTR_RET(Owner, LogAnimation, "AnimNotify_PlaySoundCue:: No owning Actor found!");

    UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(Owner);
    CHECK_NULLPTR_RET(GameInstance, LogAnimation, "AnimNotify_PlaySoundCue:: No Game Instance found!");

    GameInstance->PlayDiageticSound(SoundName, Owner, SoundLocation == FVector::ZeroVector ? Owner->GetActorLocation() : SoundLocation, VolumeMultiplier);

    ReportNoiseEvent();
}

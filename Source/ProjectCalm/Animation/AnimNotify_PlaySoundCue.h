// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_PlaySoundCue.generated.h"

#define MAX_NOISE_RANGE 5000.0f

class USoundCue;
enum EPhysicalSurface;


/**
 * 
 */
UCLASS(meta=(DisplayName="Play Sound Cue"))
class PROJECTCALM_API UAnimNotify_PlaySoundCue : public UAnimNotify
{
	GENERATED_BODY()

protected:
    UPROPERTY(EditAnywhere, Category = SFX)
    FName SoundName;

	/*If zero, a noise event won't be reported to the AI Perception System*/
    UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float BaseNoiseLevel{0.0f};

	UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float NoiseDeviation{0.0f};

	UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float Delay{0.0f};

	UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float DelayDeviation{0.0f};

	UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0", ClampMax = "1.0", UIMax = "1.0"))
	float ChanceToPlay{1.0f};

    AActor* Owner{nullptr};
	FVector SoundLocation{FVector::ZeroVector};
	float VolumeMultiplier{1.0f};
	FTimerHandle DelayTimerHandle;

public:
	UAnimNotify_PlaySoundCue(){};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

protected:
    virtual void ReportNoiseEvent();
	virtual void PlaySoundCue();
	
};

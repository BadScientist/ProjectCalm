// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlaySoundCue.h"
#include "AnimNotify_FootPlant.generated.h"

class USoundCue;
enum EPhysicalSurface;

/**
 * 
 */
UCLASS(meta=(DisplayName="Foot Plant"))
class PROJECTCALM_API UAnimNotify_FootPlant : public UAnimNotify_PlaySoundCue
{
	GENERATED_BODY()

public:
	UAnimNotify_FootPlant(){};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = Location)
	FVector FootOffset{FVector::ZeroVector};

	UPROPERTY(EditAnywhere, Category = Noise, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float AnimBlendSpaceMovementSpeed{100.0f};

	UPROPERTY(EditAnywhere, Category = SFX)
	TMap<TEnumAsByte<EPhysicalSurface>, FName> SoundMap;

	float SpeedRatio{1.0f};

protected:
	virtual void ReportNoiseEvent() override;

private:
	void PrepareFootstepSound();
	bool IsCharacterMoving();
	void CalculateSpeedRatio();
	
};

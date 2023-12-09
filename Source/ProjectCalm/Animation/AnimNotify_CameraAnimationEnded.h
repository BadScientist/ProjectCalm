// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "AnimNotify_CameraAnimationEnded.generated.h"

/**
 * 
 */
UCLASS(meta=(DisplayName="Camera Animation Ended"))
class PROJECTCALM_API UAnimNotify_CameraAnimationEnded : public UAnimNotify
{
	GENERATED_BODY()

public:
	UAnimNotify_CameraAnimationEnded(){};

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};

// Copyright 2024 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "AnimNotify_PlaySoundCue.h"
#include "AnimNotify_DealDamage.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTCALM_API UAnimNotify_DealDamage : public UAnimNotify_PlaySoundCue
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Damage{10.0f};

	UPROPERTY(EditAnywhere)
	FString DamageMessage;

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	
};

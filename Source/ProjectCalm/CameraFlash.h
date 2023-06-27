// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Equipment.h"
#include "CameraFlash.generated.h"


class USpotLightComponent;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACameraFlash : public AEquipment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Light, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;

	UPROPERTY(EditDefaultsOnly, Category = Light)
	float FlashDuration = 0.15f;
	FTimerHandle FlashTimerHandle;
	
public:	
	// Sets default values for this actor's properties
	ACameraFlash();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void PlayCameraFlash();
	float GetFlashDuration() {return FlashDuration;};

private:
	void DeactivateFlash();
	
};

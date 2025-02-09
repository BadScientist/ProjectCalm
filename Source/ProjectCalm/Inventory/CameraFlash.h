// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "Attachment.h"
#include "CameraFlash.generated.h"


class USpotLightComponent;
enum EEquipReply : uint8;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACameraFlash : public AAttachment
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
	
	// START EQUIPMENT INTERFACE
	virtual EEquipReply Equip(APlayerCharacter* OwningCharacter) override;
	// END EQUIPMENT INTERFACE
	
	virtual EEquipReply Equip_Internal(AActor* OwningActor) override;

public:
	float PlayCameraFlash();

private:
	void DeactivateFlash();
	
};

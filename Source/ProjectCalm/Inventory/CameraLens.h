// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attachment.h"
#include "CameraLens.generated.h"

class UInputAction;
class USceneCaptureComponent2D;
class UPhotoDataCollectorComponent;
struct FConvexVolume;
struct FPhotoData;
enum EEquipReply : uint8;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACameraLens : public AAttachment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UTextureRenderTarget2D> TextureTargetAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LensZoomAction;

	FEnhancedInputActionEventBinding* ZoomInputBinding;
	UPhotoDataCollectorComponent* PhotoDataCollector;
	UCameraComponent* PlayerCameraComponent;

public:
	ACameraLens();

protected:
	virtual EEquipReply Equip_Internal(AActor* OwningActor) override;
	
	// START EQUIPMENT INTERFACE
	virtual EEquipReply Equip(APlayerCharacter* OwningCharacter) override;
	// END EQUIPMENT INTERFACE

	virtual void Tick(float DeltaSeconds) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float ZoomRate = 25;  // FOV degrees/sec
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float MaxFOV = 75;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float MinFOV = 60;
	float TargetFOV;

	UTextureRenderTarget2D* CopyRenderTarget(UTextureRenderTarget2D* InRenderTarget);
	
	ULocalPlayer* GetLocalPlayer();
	TObjectPtr<UGameViewportClient> GetViewportClient(ULocalPlayer* LocalPlayer);
	FViewport* GetViewport(TObjectPtr<UGameViewportClient> ViewportClient);
	FConvexVolume GetViewFrustum();
	
protected:
	virtual void SetupPlayerControls() override;
	virtual void ResetPlayerControls() override;

public:
	void ZoomAction(const FInputActionValue& Value);
	float GetTargetFOV() {return TargetFOV;};
	USceneCaptureComponent2D* GetSceneCaptureComponent() {return SceneCaptureComponent;}
	FPhotoData CapturePhoto();
};

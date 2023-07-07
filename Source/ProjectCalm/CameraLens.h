// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "CameraLens.generated.h"

class UInputAction;
class USceneCaptureComponent2D;
class UPhotoDataCollectorComponent;
struct FConvexVolume;
struct FPhotoData;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACameraLens : public AEquipment
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	TObjectPtr <UTextureRenderTarget2D> TextureTargetAsset;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LensZoomAction;

	UPhotoDataCollectorComponent* PhotoDataCollector;
	UCameraComponent* PlayerCameraComponent;

public:
	ACameraLens();

protected:
	virtual void Equip(AActor* OwningActor, FName SocketName) override;
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

public:
	void ZoomAction(const FInputActionValue& Value);
	float GetTargetFOV() {return TargetFOV;};
	USceneCaptureComponent2D* GetSceneCaptureComponent() {return SceneCaptureComponent;}
	FPhotoData CapturePhoto();
};

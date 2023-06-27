// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "CameraLens.generated.h"

class UInputAction;
class USceneCaptureComponent2D;

/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACameraLens : public AEquipment
{
	GENERATED_BODY()

private:
	UPROPERTY(Category = Camera, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneCaptureComponent2D* SceneCaptureComponent;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LensZoomAction;

public:
	ACameraLens();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float ZoomRate = 5;  // FOV degrees/sec
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float MaxFOV = 75;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = CameraLens, meta = (AllowPrivateAccess = "true"))
	float MinFOV = 60;

	float TargetFOV;
	
protected:
	virtual void SetupPlayerControls() override;

public:
	void ZoomAction(const FInputActionValue& Value);
	float GetTargetFOV() {return TargetFOV;};
	USceneCaptureComponent2D* GetSceneCaptureComponent() {return SceneCaptureComponent;}
};

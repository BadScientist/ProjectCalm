// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Equipment.h"
#include "PhotoCameraEquipment.generated.h"

class UImage;
class UTextureRenderTarget2D;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class ACameraFlash;
class ACameraLens;
struct FInputActionValue;
struct FPhotoData;

UCLASS()
class PROJECTCALM_API APhotoCameraEquipment : public AEquipment
{
	GENERATED_BODY()

private:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* RaiseAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Animation, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* LowerAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = PhotoCamera, meta = (AllowPrivateAccess = "true"))
	TArray<FPhotoData> Photos;
	TSubclassOf<class UUserWidget> CameraHUDClass;
	UUserWidget* CameraHUD;
	UImage* LastPhotoImageWidget;

	float DefaultAnimationBlendTime = 0.0f;
	FTimerHandle AnimationTimerHandle;	
	FTimerHandle BlendViewTimerHandle;
	FTimerHandle PhotoDelayTimerHandle;
	
	int32 MaxPhotos = 12;
	
public:	
	// Sets default values for this actor's properties
	APhotoCameraEquipment();

protected:
	ACameraFlash* AttachedCameraFlash = nullptr;
	ACameraLens* AttachedCameraLens = nullptr;
	
	virtual void Equip(AActor* OwningActor, FName SocketName) override;

	void RaiseCamera();
	void EnterCameraView();
	void ActivateRaisedCameraMode();
	void DeactivateRaisedCameraMode();
	void ExitCameraView();
	void LowerCamera();
	void EnterDefaultState();

	float ActivateCameraFlash();
	void TakePhoto();

	bool IsAnimationRunning();

public:
	void PrimaryAction(const FInputActionValue& Value) override;
	void SecondaryAction(const FInputActionValue& Value) override;
	void SetAttachedCameraFlash(ACameraFlash* CameraFlash) {AttachedCameraFlash = CameraFlash;};
	void SetAttachedCameraLens(ACameraLens* CameraLens) {AttachedCameraLens = CameraLens;};

	FPhotoData GetLastPhoto();

private:
	float BlendViewToPhotoCamera();
	float BlendViewToPlayerCharacter();
	void PauseTimers();
	void DisplayCameraHUD(bool bDisplay);
	void DisplayLastPhoto();
	void LogPhotoData(FPhotoData Photo);

};

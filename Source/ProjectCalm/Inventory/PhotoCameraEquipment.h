// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Equipment.h"
#include "PhotoCameraEquipment.generated.h"

class UImage;
class UTextBlock;
class UTextureRenderTarget2D;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class ACameraFlash;
class ACameraLens;
struct FInputActionValue;
struct FPhotoData;


UENUM()
enum ECameraState
{	
    DEFAULT		=0	UMETA(DisplayName = "Default"),
	RAISING		=1	UMETA(DisplayName = "Raising"),
	BLENDING_IN	=2	UMETA(DisplayName = "Blending In"),
	READY		=3	UMETA(DisplayName = "Ready"),
	BLENDING_OUT=4	UMETA(DisplayName = "Blending Out"),
	LOWERING	=5	UMETA(DisplayName = "Lowering")
};


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
	int32 MaxPhotos = 12;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PhotoCamera, meta = (AllowPrivateAccess = "true"))
	TArray<FPhotoData> Photos;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = HUD, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> CameraHUD;
	UUserWidget* CameraHUDInstance;
	UImage* LastPhotoImageWidget;
	UTextBlock* FilmTextWidget;

	FTimerHandle BlendViewTimerHandle;
	FTimerHandle PhotoDelayTimerHandle;
	TEnumAsByte<ECameraState> CameraState{ECameraState::DEFAULT};
	
public:	
	// Sets default values for this actor's properties
	APhotoCameraEquipment();

protected:
	ACameraFlash* AttachedCameraFlash = nullptr;
	ACameraLens* AttachedCameraLens = nullptr;
	
	virtual void Equip(AActor* OwningActor, FName SocketName) override;

	void OnSecondaryButtonDown();
	void PlayRaiseLowerAnimation(bool bRaise=true);
	void EnterCameraView();
	void ActivateRaisedCameraMode();
	void OnSecondaryButtonUp();
	void OnBlendViewTimerElapsed();
	void ExitCameraView();
	void LowerCamera();
	void EnterDefaultState();
	void SetCameraState(ECameraState InState);

	float ActivateCameraFlash();
	void TakePhoto();

public:
	void PrimaryAction(const FInputActionValue& Value) override;
	void SecondaryAction(const FInputActionValue& Value) override;
	void SetAttachedCameraFlash(ACameraFlash* CameraFlash) {AttachedCameraFlash = CameraFlash;};
	void SetAttachedCameraLens(ACameraLens* CameraLens) {AttachedCameraLens = CameraLens;};
	void OnAnimationEnded();

	FPhotoData GetLastPhoto();

private:
	float BlendViewToPhotoCamera();
	float BlendViewToPlayerCharacter();
	void DisplayCameraHUD(bool bDisplay);
	void UpdateHUDOverlay();
	void DisplayLastPhoto();
	void LogPhotoData(FPhotoData Photo);

};

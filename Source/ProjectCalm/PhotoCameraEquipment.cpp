// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoCameraEquipment.h"
#include "CameraFlash.h"
#include "CameraLens.h"
#include "PlayerCharacter.h"
#include "InfoFlagNameDefinitions.h"

#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
APhotoCameraEquipment::APhotoCameraEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// TODO: Create Camera class and move this there.
    ConstructorHelpers::FClassFinder<UUserWidget> CameraHUDBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_CameraHUD"));
    if (!ensure(CameraHUDBPClass.Class != nullptr)) {return;}
    CameraHUDClass = CameraHUDBPClass.Class;

	PrimaryActionCooldown = 0.5f;
}

void APhotoCameraEquipment::Equip(AActor* OwningActor, FName SocketName)
{
	Super::Equip(OwningActor, SocketName);

	SetPlayerFlag(FLAG_PLAYER_HAS_CAMERA, true);
}

void APhotoCameraEquipment::RaiseCamera()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: RaiseCamera"));

	PauseTimers();

	APlayerCharacter* OwningCharacter = GetPlayerCharacter();
	if (OwningCharacter != nullptr && RaiseAnimation != nullptr)
	{
		if (!GetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_END))
		{
			float AnimationTimeRemaining = GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle);
			if (AnimationTimeRemaining >= 0) {RaiseAnimation->BlendIn = AnimationTimeRemaining;}
			else {RaiseAnimation->BlendIn = DefaultAnimationBlendTime;}

			float CallbackTime = OwningCharacter->PlayAnimMontage(RaiseAnimation, 1.0f) - 0.25f;  // Early callback prevents animation blending issues
			GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &APhotoCameraEquipment::EnterCameraView, CallbackTime);
			SetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_START, true);
		}
		else {EnterCameraView();}
	}
}

void APhotoCameraEquipment::EnterCameraView()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: EnterCameraView"));
	SetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_END, true);
	
	float BlendTime = BlendViewToPhotoCamera();	
	GetWorldTimerManager().SetTimer(BlendViewTimerHandle, this, &APhotoCameraEquipment::ActivateRaisedCameraMode, BlendTime);
	
	SetPlayerFlag(FLAG_CAMERA_BLENDING, true);
}

void APhotoCameraEquipment::ActivateRaisedCameraMode()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: ActivateRaisedCameraMode"));
	SetPlayerFlag(FLAG_CAMERA_BLENDING, false);
	
	DisplayCameraHUD(true);
	
	SetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE, true);
}

void APhotoCameraEquipment::DeactivateRaisedCameraMode()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: DeactivateRaisedCameraMode"));
	PauseTimers();
	
	DisplayCameraHUD(false);
	
	ExitCameraView();
}

void APhotoCameraEquipment::ExitCameraView()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: ExitCameraView"));
	if (GetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE))
	{
		float BlendTime = BlendViewToPlayerCharacter();
		GetWorldTimerManager().SetTimer(BlendViewTimerHandle, this, &APhotoCameraEquipment::LowerCamera, BlendTime);
		SetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE, false);
	}
	else {LowerCamera();}
}

void APhotoCameraEquipment::LowerCamera()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: LowerCamera"));
	if (!GetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_START)) {return;}

	APlayerCharacter* OwningCharacter = GetPlayerCharacter();
	if (OwningCharacter != nullptr && LowerAnimation != nullptr)
	{
		float AnimationTimeRemaining = GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle);
		if (AnimationTimeRemaining >= 0) {LowerAnimation->BlendIn = AnimationTimeRemaining;}
		else {LowerAnimation->BlendIn = DefaultAnimationBlendTime;}

		GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &APhotoCameraEquipment::EnterDefaultState, OwningCharacter->PlayAnimMontage(LowerAnimation, 1.0f));
		SetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_END, false);
	}
}

void APhotoCameraEquipment::EnterDefaultState()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCamera: EnterDefaultState"));
	SetPlayerFlag(FLAG_CAMERA_RAISE_ANIMATION_START, false);
}

bool APhotoCameraEquipment::IsAnimationRunning()
{
	return GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle) > 0;
}

void APhotoCameraEquipment::PrimaryAction(const FInputActionValue& Value)
{	
	if (!GetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE)) {return;}
	if (Value.Get<bool>() && !bPrimaryActionOnCooldown)
	{
		SetPrimaryActionOnCooldown();
		GEngine->AddOnScreenDebugMessage(0, 1, FColor::Green, "SNAP");
		if (AttachedCameraFlash != nullptr)
		{
			float WaitTime = AttachedCameraFlash->GetFlashDuration() + PrimaryActionCooldown;
    		GetWorldTimerManager().SetTimer(PrimaryActionCooldownTimerHandle, this, &APhotoCameraEquipment::ClearPrimaryActionOnCooldown, WaitTime);
			AttachedCameraFlash->PlayCameraFlash();
		}
	}
}

void APhotoCameraEquipment::SecondaryAction(const FInputActionValue& Value)
{
	bool bValue = Value.Get<bool>();

	if (bValue)
	{
		UE_LOG(LogTemp, Error, TEXT("-----RMB DOWN-----"));
		RaiseCamera();
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("-----RMB UP-----"));
		DeactivateRaisedCameraMode();
	}
}

float APhotoCameraEquipment::BlendViewToPhotoCamera()
{
	if (AttachedCameraLens == nullptr) {return 0.0f;}

	USceneCaptureComponent2D* LensCaptureComp = AttachedCameraLens->GetSceneCaptureComponent();
	if (LensCaptureComp == nullptr) {return 0.0f;}

	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacter())
	{
		return PlayerCharacter->BlendViewToSceneCaptureComponent(LensCaptureComp);
	}

	return 0.0f;
}

float APhotoCameraEquipment::BlendViewToPlayerCharacter()
{
	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacter())
	{
		return PlayerCharacter->ResetCameraLocation();
	}

	return 0.0f;
}

void APhotoCameraEquipment::PauseTimers()
{
	GetWorldTimerManager().PauseTimer(BlendViewTimerHandle);
	GetWorldTimerManager().PauseTimer(AnimationTimerHandle);
}

void APhotoCameraEquipment::DisplayCameraHUD(bool bDisplay)
{
	if (bDisplay)
	{
		UGameInstance* GameInstance = GetGameInstance();
		CameraHUD = CreateWidget<UUserWidget>(GameInstance, CameraHUDClass, TEXT("CameraHUD"));
		if (CameraHUD != nullptr && !CameraHUD->IsInViewport()) {CameraHUD->AddToViewport();}
	}
	else
	{
		if (CameraHUD != nullptr && CameraHUD->IsInViewport()) {CameraHUD->RemoveFromParent();}
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoCameraEquipment.h"
#include "CameraFlash.h"
#include "CameraLens.h"
#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

#define FLAG_RAISED_START TEXT("StartRaised")
#define FLAG_RAISED_END TEXT("EndRaised")

// Sets default values
APhotoCameraEquipment::APhotoCameraEquipment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// TODO: Create Camera class and move this there.
    ConstructorHelpers::FClassFinder<UUserWidget> CameraHUDBPClass(TEXT("/Game/ProjectCalm/Blueprints/UI/WBP_CameraHUD"));
    if (!ensure(CameraHUDBPClass.Class != nullptr)) {return;}
    CameraHUDClass = CameraHUDBPClass.Class;

	EquipmentFlags.Add(FEquipmentFlag(FLAG_RAISED_START, false));
	EquipmentFlags.Add(FEquipmentFlag(FLAG_RAISED_END, false));

	PrimaryActionCooldown = 0.5f;
}

// Called when the game starts or when spawned
void APhotoCameraEquipment::BeginPlay()
{
	Super::BeginPlay();
	//
}

void APhotoCameraEquipment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//
}

void APhotoCameraEquipment::Equip(AActor* OwningActor, FName SocketName)
{
	Super::Equip(OwningActor, SocketName);
	SetPlayerHasCamera(true);
}

void APhotoCameraEquipment::SetPlayerHasCamera(bool bValue)
{
	if (APlayerCharacter* OwningCharacter = GetPlayerCharacter())
	{
		OwningCharacter->SetHasCamera(bValue);
	}
}

void APhotoCameraEquipment::RaiseCamera()
{
	PauseTimers();

	APlayerCharacter* OwningCharacter = GetPlayerCharacter();
	if (OwningCharacter != nullptr && RaiseAnimation != nullptr)
	{
		float AnimationTimeRemaining = GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle);
		if (AnimationTimeRemaining >= 0) {RaiseAnimation->BlendIn = AnimationTimeRemaining;}
		else {RaiseAnimation->BlendIn = DefaultAnimationBlendTime;}

		GetWorldTimerManager().SetTimer(AnimationTimerHandle, this, &APhotoCameraEquipment::ActivateRaisedCameraMode, OwningCharacter->PlayAnimMontage(RaiseAnimation, 1.0f));
		SetFlag(FLAG_RAISED_START, true);
	}
}

void APhotoCameraEquipment::ActivateRaisedCameraMode()
{
	DisplayCameraHUD();
	
	float BlendTime = BlendViewToPhotoCamera();	
	GetWorldTimerManager().SetTimer(BlendViewTimerHandle, BlendTime, false);
	
	SetFlag(FLAG_RAISED_END, true);
}

void APhotoCameraEquipment::LowerCamera()
{
	PauseTimers();
	
	float BlendTime = BlendViewToPlayerCharacter();
	GetWorldTimerManager().SetTimer(BlendViewTimerHandle, this, &APhotoCameraEquipment::DeactivateRaisedCameraMode, BlendTime);
}

void APhotoCameraEquipment::DeactivateRaisedCameraMode()
{
	APlayerCharacter* OwningCharacter = GetPlayerCharacter();
	if (OwningCharacter != nullptr && LowerAnimation != nullptr)
	{
		float AnimationTimeRemaining = GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle);
		if (AnimationTimeRemaining >= 0) {LowerAnimation->BlendIn = AnimationTimeRemaining;}
		else {LowerAnimation->BlendIn = DefaultAnimationBlendTime;}

		GetWorldTimerManager().SetTimer(AnimationTimerHandle, OwningCharacter->PlayAnimMontage(LowerAnimation, 1.0f), false);
		SetFlag(FLAG_RAISED_END, false);
	}
	
	if (CameraHUD != nullptr && CameraHUD->IsInViewport()) {CameraHUD->RemoveFromParent();}
	SetFlag(FLAG_RAISED_START, false);
}

bool APhotoCameraEquipment::IsAnimationRunning()
{
	return GetWorldTimerManager().GetTimerRemaining(AnimationTimerHandle) > 0;
}

void APhotoCameraEquipment::PrimaryAction(const FInputActionValue& Value)
{	
	if (!GetFlagByName(FLAG_RAISED_END)) {return;}
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
		RaiseCamera();
	}
	else
	{
		LowerCamera();
	}
}

float APhotoCameraEquipment::BlendViewToPhotoCamera()
{
	if (AttachedCameraLens == nullptr) {return 0.0f;}

	USceneCaptureComponent2D* LensCaptureComp = AttachedCameraLens->GetSceneCaptureComponent();
	if (LensCaptureComp == nullptr) {return 0.0f;}

	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacter())
	{
		PlayerCharacter->BlendViewToSceneCaptureComponent(LensCaptureComp);
		return PlayerCharacter->GetViewBlenderBlendTime();
	}

	return 0.0f;
}

float APhotoCameraEquipment::BlendViewToPlayerCharacter()
{
	if (APlayerCharacter* PlayerCharacter = GetPlayerCharacter())
	{
		PlayerCharacter->ResetCameraLocation();
		return PlayerCharacter->GetViewBlenderBlendTime();
	}

	return 0.0f;
}

void APhotoCameraEquipment::PauseTimers()
{
	GetWorldTimerManager().PauseTimer(BlendViewTimerHandle);
	GetWorldTimerManager().PauseTimer(AnimationTimerHandle);
}

void APhotoCameraEquipment::DisplayCameraHUD()
{
	UGameInstance* GameInstance = GetGameInstance();
	CameraHUD = CreateWidget<UUserWidget>(GameInstance, CameraHUDClass, TEXT("CameraHUD"));
	if (CameraHUD != nullptr && !CameraHUD->IsInViewport()) {CameraHUD->AddToViewport();}
}

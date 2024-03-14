// Fill out your copyright notice in the Description page of Project Settings.


#include "PhotoCameraEquipment.h"
#include "CameraFlash.h"
#include "CameraLens.h"
#include "EquipReply.h"
#include "EquipperInterface.h"
#include "MeshSockets.h"
#include "ProjectCalm/ProjectCalmGameInstance.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/Photos/PhotoSubjectData.h"
#include "ProjectCalm/Photos/PhotoSubjectPointOfInterest.h"
#include "ProjectCalm/Characters/Player/InfoFlagNameDefinitions.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Blueprint/WidgetTree.h"
#include "InputActionValue.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"


APhotoCameraEquipment::APhotoCameraEquipment()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActionCooldown = 0.5f;
}

EEquipReply APhotoCameraEquipment::Equip_Internal(AActor* OwningActor)
{
	EEquipReply Response = Super::Equip_Internal(OwningActor);

	if (Response == EEquipReply::SUCCESS) {SetPlayerFlag(FLAG_PLAYER_HAS_CAMERA, true);}

	return Response;
}

void APhotoCameraEquipment::Unequip()
{
	SetPlayerFlag(FLAG_PLAYER_HAS_CAMERA, false);

	if (AttachedCameraFlash != nullptr && !AttachedCameraFlash->IsActorBeingDestroyed())
	{
		if (IEquipmentInterface* EquippedFlash = Cast<IEquipmentInterface>(AttachedCameraFlash)) {EquippedFlash->Unequip();}
	}
	if (AttachedCameraLens != nullptr && !AttachedCameraLens->IsActorBeingDestroyed())
	{
		if (IEquipmentInterface* EquippedLens = Cast<IEquipmentInterface>(AttachedCameraLens)) {EquippedLens->Unequip();}
	}

	Super::Unequip();
}

bool APhotoCameraEquipment::AttachEquipment(IEquipmentInterface *Equipment, FName SocketName)
{
	AActor* EquipmentActor{nullptr};
	if (ACameraFlash* CameraFlash = Cast<ACameraFlash>(Equipment->_getUObject()))
	{
		AttachedCameraFlash = CameraFlash;
		EquipmentActor = CameraFlash;
	}
	else if (ACameraLens* CameraLens = Cast<ACameraLens>(Equipment->_getUObject()))
	{
		AttachedCameraLens = CameraLens;
		EquipmentActor = CameraLens;
	}

	if (EquipmentActor != nullptr)
	{
		EquipmentActor->AttachToComponent(GetEquipmentMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		return true;
	}

    return false;
}

void APhotoCameraEquipment::RemoveEquipment(IEquipmentInterface *Equipment)
{
	if (ACameraFlash* CameraFlash = Cast<ACameraFlash>(Equipment->_getUObject()))
	{
		AttachedCameraFlash = nullptr;
	}
	else if (ACameraLens* CameraLens = Cast<ACameraLens>(Equipment->_getUObject()))
	{
		AttachedCameraLens = nullptr;
	}
}

void APhotoCameraEquipment::OnSecondaryButtonDown()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCameraEquipment:: OnSecondaryButtonDown()"));

	if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this)) {PlayerCharacter->HideHUD();}

	GetWorldTimerManager().PauseTimer(BlendViewTimerHandle);

	switch (CameraState)
	{
	case ECameraState::DEFAULT:
		PlayRaiseLowerAnimation();
		break;
	case ECameraState::RAISING:
		PlayRaiseLowerAnimation();
		break;
	case ECameraState::BLENDING_IN:
		EnterCameraView();
		break;
	case ECameraState::READY:
		EnterCameraView();
		break;
	case ECameraState::BLENDING_OUT:
		EnterCameraView();
		break;
	case ECameraState::LOWERING:
		PlayRaiseLowerAnimation();
		break;
	default:
		break;
	}
}

void APhotoCameraEquipment::PlayRaiseLowerAnimation(bool bRaise)
{
	SetCameraState(bRaise ? ECameraState::RAISING : ECameraState::LOWERING);
}

void APhotoCameraEquipment::EnterCameraView()
{
	if (CameraState != ECameraState::READY)
	{
		float BlendTime = BlendViewToPhotoCamera();
		if (!FMath::IsNearlyZero(BlendTime))
		{
			GetWorldTimerManager().SetTimer(BlendViewTimerHandle, this, &APhotoCameraEquipment::OnBlendViewTimerElapsed, BlendTime);
			return;
		}
	}

	OnBlendViewTimerElapsed();
}

void APhotoCameraEquipment::ActivateRaisedCameraMode()
{
	DisplayCameraHUD(true);	
	SetCameraState(ECameraState::READY);
}

void APhotoCameraEquipment::OnSecondaryButtonUp()
{
	GetWorldTimerManager().PauseTimer(BlendViewTimerHandle);

	switch (CameraState)
	{
	case ECameraState::DEFAULT:
		PlayRaiseLowerAnimation(false);
		break;
	case ECameraState::RAISING:
		PlayRaiseLowerAnimation(false);
		break;
	case ECameraState::BLENDING_IN:
		ExitCameraView();
		break;
	case ECameraState::READY:
		ExitCameraView();
		break;
	case ECameraState::BLENDING_OUT:
		ExitCameraView();
		break;
	case ECameraState::LOWERING:
		PlayRaiseLowerAnimation(false);
		break;
	default:
		break;
	}
}

void APhotoCameraEquipment::OnAnimationEnded()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCameraEquipment:: OnAnimationEnded()"));
	switch (CameraState)
	{
	case ECameraState::RAISING:
		EnterCameraView();
		break;
	case ECameraState::LOWERING:
		EnterDefaultState();
		break;	
	default:
		break;
	}
}

void APhotoCameraEquipment::OnBlendViewTimerElapsed()
{
	switch (CameraState)
	{
	case ECameraState::BLENDING_IN:
		ActivateRaisedCameraMode();
		break;
	case ECameraState::READY:
		ActivateRaisedCameraMode();
		break;
	case ECameraState::BLENDING_OUT:
		LowerCamera();
		break;	
	default:
		break;
	}
}

void APhotoCameraEquipment::ExitCameraView()
{
	DisplayCameraHUD(false);

	float BlendTime = BlendViewToPlayerCharacter();
	SetCameraState(ECameraState::BLENDING_OUT);
	if (!FMath::IsNearlyZero(BlendTime)) {GetWorldTimerManager().SetTimer(BlendViewTimerHandle, this, &APhotoCameraEquipment::OnBlendViewTimerElapsed, BlendTime);}
	else {OnBlendViewTimerElapsed();}
}

void APhotoCameraEquipment::LowerCamera()
{
	PlayRaiseLowerAnimation(false);
}

void APhotoCameraEquipment::EnterDefaultState()
{
	UE_LOG(LogTemp, Warning, TEXT("PhotoCameraEquipment:: EnterDefaultState()"));
	if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this)) {PlayerCharacter->ShowHUD();}
	SetCameraState(ECameraState::DEFAULT);
}

void APhotoCameraEquipment::SetCameraState(ECameraState InState)
{
	FString StateString = InState == ECameraState::DEFAULT ? FString("Default") : \
		InState == ECameraState::RAISING ? FString("Raising") : \
		InState == ECameraState::BLENDING_IN ? FString("Blending In") : \
		InState == ECameraState::READY ? FString("Ready") : \
		InState == ECameraState::BLENDING_OUT ? FString("Blending Out") : \
		InState == ECameraState::LOWERING ? FString("Lowering") : FString("Error");
	UE_LOG(LogTemp, Warning, TEXT("CameraState = %s"), *StateString);

	SetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE, InState == ECameraState::READY);
	SetPlayerFlag(FLAG_CAMERA_RAISED,
		InState == ECameraState::RAISING ||
		InState == ECameraState::BLENDING_IN ||
		InState == ECameraState::READY);
	CameraState = InState;
}

float APhotoCameraEquipment::ActivateCameraFlash()
{
    if (AttachedCameraFlash != nullptr) {return AttachedCameraFlash->PlayCameraFlash();}

	return 0.0f;
}

void APhotoCameraEquipment::TakePhoto()
{
	if (AttachedCameraLens == nullptr)
	{
		APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
		CHECK_NULLPTR_RET(PlayerCharacter, LogEquipment, "PhotoCameraEquipment::No PlayerCharacter found!");
		PlayerCharacter->NotifyPlayer(FString("The camera requires a lens!"));
		return;
	}
	if (Photos.Num() >= MaxPhotos) {return;}
	
	FPhotoData NewPhoto = AttachedCameraLens->CapturePhoto();
	Photos.Add(NewPhoto);

	UpdateHUDOverlay();
}

void APhotoCameraEquipment::PrimaryAction(const FInputActionValue& Value)
{	
	if (CameraState != ECameraState::READY) {return;}

	if (Value.Get<bool>() && !bPrimaryActionOnCooldown)
	{
		SetPrimaryActionOnCooldown();
		
		float FlashDuration = ActivateCameraFlash();
		
		GetWorldTimerManager().SetTimer(PrimaryActionCooldownTimerHandle, this, &APhotoCameraEquipment::ClearPrimaryActionOnCooldown, FlashDuration + PrimaryActionCooldown);
		
		if (!FMath::IsNearlyZero(FlashDuration)) {GetWorldTimerManager().SetTimer(PhotoDelayTimerHandle, this, &APhotoCameraEquipment::TakePhoto, FlashDuration/2);}
		else {TakePhoto();}
	}
}

void APhotoCameraEquipment::SecondaryAction(const FInputActionValue& Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogEquipment, "PhotoCameraEquipment:: No Game Instance found!");
	if (GameInstance->IsPopupOpen()) {return;}

	bool bValue = Value.Get<bool>();

	if (bValue)
	{
		UE_LOG(LogInput, Display, TEXT("------------------------------------------------------------RMB DOWN------------------------------------------------------------"));
		OnSecondaryButtonDown();
	}
	else
	{
		UE_LOG(LogInput, Display, TEXT("-------------------------------------------------------------RMB UP-------------------------------------------------------------"));
		OnSecondaryButtonUp();
	}
}

FPhotoData APhotoCameraEquipment::GetLastPhoto()
{
	if (Photos.IsEmpty()) {return FPhotoData();}
	return Photos[Photos.Num() - 1];
}

float APhotoCameraEquipment::BlendViewToPhotoCamera()
{
	SetCameraState(ECameraState::BLENDING_IN);
	if (AttachedCameraLens == nullptr)
	{
		APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this);
		CHECK_NULLPTR_RETVAL(PlayerCharacter, LogEquipment, "PhotoCameraEquipment::No PlayerCharacter found!", 0.0f);
		PlayerCharacter->NotifyPlayer(FString("The camera requires a lens!"));
		return 0.0f;
	}

	USceneCaptureComponent2D* LensCaptureComp = AttachedCameraLens->GetSceneCaptureComponent();
	if (LensCaptureComp == nullptr) {return 0.0f;}

	if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this))
	{
		return PlayerCharacter->BlendViewToSceneCaptureComponent(LensCaptureComp);
	}

	return 0.0f;
}

float APhotoCameraEquipment::BlendViewToPlayerCharacter()
{
	if (APlayerCharacter* PlayerCharacter = PCPlayerStatics::GetPlayerCharacter(this)) {return PlayerCharacter->ResetCameraLocation();}

	return 0.0f;
}

void APhotoCameraEquipment::DisplayCameraHUD(bool bDisplay)
{
	if (bDisplay)
	{
		UGameInstance* GameInstance = GetGameInstance();
		if (CameraHUD != nullptr) {CameraHUDInstance = CreateWidget<UUserWidget>(GameInstance, CameraHUD, TEXT("CameraHUD"));}
		if (CameraHUDInstance != nullptr && !CameraHUDInstance->IsInViewport())
		{
			CameraHUDInstance->AddToViewport();
			LastPhotoImageWidget = CameraHUDInstance->WidgetTree->FindWidget<UImage>(TEXT("LastPhotoImage"));
			FilmTextWidget = CameraHUDInstance->WidgetTree->FindWidget<UTextBlock>(TEXT("FilmText"));
			UpdateHUDOverlay();
		}
	}
	else
	{
		if (CameraHUDInstance != nullptr && CameraHUDInstance->IsInViewport())
		{
			CameraHUDInstance->RemoveFromParent();
			CameraHUDInstance = nullptr;
		}
	}
}

void APhotoCameraEquipment::UpdateHUDOverlay()
{
	if (FilmTextWidget != nullptr)
	{
		FString FilmString = FString::Printf(TEXT("%i/%i"), Photos.Num(), MaxPhotos);
		FilmTextWidget->SetText(FText::FromString(FilmString));
	}
	DisplayLastPhoto();
}

void APhotoCameraEquipment::DisplayLastPhoto()
{
	if (Photos.IsEmpty()) {return;}

	CHECK_NULLPTR_RET(LastPhotoImageWidget, LogEquipment, "PhotoCameraEquipment:: No LastPhotoImageWidget set!");

	FPhotoData LastPhoto = GetLastPhoto();
	UTextureRenderTarget2D* PhotoImage = LastPhoto.Image;

	CHECK_NULLPTR_RET(PhotoImage, LogEquipment, "PhotoCameraEquipment:: LastPhoto has no image!");
	if (!PhotoImage->IsValidLowLevel())
	{
		UE_LOG(LogEquipment, Error, TEXT("PhotoCameraEquipment:: PhotoImage is invalid!"));
		return;
	}

	UMaterialInstanceDynamic* PhotoRenderMat = LastPhotoImageWidget->GetDynamicMaterial();
	CHECK_NULLPTR_RET(PhotoRenderMat, LogEquipment, "PhotoCameraEquipment:: LastPhotoImageWidget has no DynamicMaterial!");

	PhotoRenderMat->SetTextureParameterValue(TEXT("PhotoRender"), PhotoImage);
	FVector2D DisplaySize = FVector2D(PhotoImage->SizeX/10, PhotoImage->SizeY/10);
	LastPhotoImageWidget->SetDesiredSizeOverride(DisplaySize);
	LastPhotoImageWidget->SetVisibility(ESlateVisibility::Visible);

	LogPhotoData(LastPhoto);
}

void APhotoCameraEquipment::LogPhotoData(FPhotoData Photo)
{
	FString PhotoString = FString::Printf(TEXT("PHOTOCAMERA::Photo taken at %s:\nSubjects: "), *(Photo.TimeTaken.ToString()));
	float Score = 0;
	for (FPhotoSubjectData Subject : Photo.Subjects)
	{
		PhotoString.Append(SubjectName::SubjectNameEnumToString(Subject.Name));
		PhotoString.Append(FString(" | "));
		
		for (FPhotoSubjectPointOfInterest PointOfInterest : Subject.PointsOfInterest)
		{
			if (PointOfInterest.IsVisible()) {Score += PointOfInterest.ScoreValue;}
		}
	}
	PhotoString.Append(FString::Printf(TEXT("\nTotal Score: %f"), Score));
	UE_LOG(LogEquipment, Display, TEXT("%s"), *PhotoString);
}

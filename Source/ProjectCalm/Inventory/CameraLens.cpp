// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraLens.h"
#include "EquipReply.h"
#include "MeshSockets.h"
#include "EquipperInterface.h"
#include "ProjectCalm/Photos/PhotoDataCollectorComponent.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"
#include "ProjectCalm/Characters/Player/InfoFlagNameDefinitions.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"


ACameraLens::ACameraLens()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetSocket = SOCKET_CAMERA_LENS;

	PhotoDataCollector = CreateDefaultSubobject<UPhotoDataCollectorComponent>(TEXT("PhotoDataCollectorComponent"));
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(GetEquipmentMesh());
    SceneCaptureComponent->SetRelativeLocation(FVector(0.0f, 2.0f, 0.0f));
    SceneCaptureComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

EEquipReply ACameraLens::Equip_Internal(AActor* OwningActor)
{
    EEquipReply Response = Super::Equip_Internal(OwningActor);
    if (Response != EEquipReply::SUCCESS) {return EEquipReply::FAILED_NO_CAMERA;}
    
    TargetFOV = MaxFOV;
    
    PlayerCameraComponent = PCPlayerStatics::GetPlayerCamera(this);

    if (SceneCaptureComponent == nullptr) {return EEquipReply::FAILED_DEFAULT;}
    SceneCaptureComponent->FOVAngle = TargetFOV;

    // Set TextureTarget size based on Screen Resolution    
    FIntPoint ScreenResolution = FIntPoint(1280, 720);
    if (GEngine) {ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();}    
    
    if (SceneCaptureComponent->TextureTarget == nullptr) {return EEquipReply::FAILED_DEFAULT;}
    SceneCaptureComponent->TextureTarget->InitAutoFormat(ScreenResolution.X, ScreenResolution.Y);
    SceneCaptureComponent->TextureTarget->UpdateResourceImmediate();

    return EEquipReply::SUCCESS;
}

EEquipReply ACameraLens::Equip(APlayerCharacter* OwningCharacter)
{
    IEquipmentInterface* EquippedItemInterface = OwningCharacter->GetEquippedItem();
    CHECK_NULLPTR_RETVAL(EquippedItemInterface, LogEquipment, "CameraLens:: Player has no equipped item!", EEquipReply::FAILED_NO_CAMERA);

    AActor* EquippedItem = Cast<AActor>(EquippedItemInterface->_getUObject());
    OwningPlayerCharacter = OwningCharacter;
    return Equip_Internal(EquippedItem);
}

void ACameraLens::Tick(float DeltaSeconds)
{
    // Only alter FOV when camera is fully raised
    if (GetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE))
    {
        if (PlayerCameraComponent != nullptr) {PlayerCameraComponent->SetFieldOfView(TargetFOV);}
        SceneCaptureComponent->FOVAngle = TargetFOV;
    }    
}

UTextureRenderTarget2D* ACameraLens::CopyRenderTarget(UTextureRenderTarget2D *InRenderTarget)
{
    UTextureRenderTarget2D* Result = UKismetRenderingLibrary::CreateRenderTarget2D(
        this,
        InRenderTarget->SizeX,
        InRenderTarget->SizeY,
        InRenderTarget->RenderTargetFormat,
        InRenderTarget->ClearColor,
        InRenderTarget->bAutoGenerateMips);
    Result->TargetGamma = InRenderTarget->TargetGamma;
    Result->NeverStream = InRenderTarget->NeverStream;

    return Result;
}

ULocalPlayer* ACameraLens::GetLocalPlayer()
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogEquipment, "CameraLens:: Could not get World!", nullptr);
    return World->GetFirstLocalPlayerFromController();
}

TObjectPtr<UGameViewportClient> ACameraLens::GetViewportClient(ULocalPlayer *LocalPlayer)
{
    if (LocalPlayer == nullptr) {return TObjectPtr<UGameViewportClient>();}
    return LocalPlayer->ViewportClient;
}

FViewport *ACameraLens::GetViewport(TObjectPtr<UGameViewportClient> ViewportClient)
{
    if (!ViewportClient) {return nullptr;}
    return ViewportClient->Viewport;
}

FConvexVolume ACameraLens::GetViewFrustum()
{
    ULocalPlayer* LocalPlayer = GetLocalPlayer();
    TObjectPtr<UGameViewportClient> ViewportClient = GetViewportClient(LocalPlayer);
    FViewport* Viewport = GetViewport(ViewportClient);
    CHECK_NULLPTR_RETVAL(Viewport, LogEquipment, "CameraLens:: Could not get Viewport!", FConvexVolume());    
    
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RETVAL(World, LogEquipment, "CameraLens:: Could not get World!", FConvexVolume());
    FSceneInterface* SceneInterface = World->Scene;
    FSceneViewFamilyContext ViewFamilyContext = FSceneViewFamily::ConstructionValues(Viewport, SceneInterface, ViewportClient->EngineShowFlags).SetRealtimeUpdate(true);
    
    FVector ViewLocation;
    FRotator ViewRotation;
    FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamilyContext, ViewLocation, ViewRotation, Viewport);
    CHECK_NULLPTR_RETVAL(SceneView, LogEquipment, "CameraLens:: Could not get SceneView!", FConvexVolume());

    return SceneView->ViewFrustum;
}

void ACameraLens::SetupPlayerControls()
{
    Super::SetupPlayerControls();    

	if (LensZoomAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
		{
			ZoomInputBinding = &(EnhancedInputComponent->BindAction(LensZoomAction, ETriggerEvent::Triggered, this, &ACameraLens::ZoomAction));
		}
	}
}

void ACameraLens::ResetPlayerControls()
{
	if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
	{
		if (ZoomInputBinding != nullptr)
		{
			EnhancedInputComponent->RemoveActionBindingForHandle(ZoomInputBinding->GetHandle());
			ZoomInputBinding = nullptr;
		}
    }    

    Super::ResetPlayerControls();
}

void ACameraLens::ZoomAction(const FInputActionValue& Value)
{
    // Only alter FOV when camera is fully raised
    if (!GetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE)) {return;}

    UWorld* World = GetWorld();
    float DeltaSeconds = World == nullptr ? 0.01f : World->GetDeltaSeconds();
    TargetFOV -= Value.Get<float>() * DeltaSeconds * ZoomRate;
    TargetFOV = FMath::Clamp(TargetFOV, MinFOV, MaxFOV);
}

FPhotoData ACameraLens::CapturePhoto()
{
    if (SceneCaptureComponent == nullptr || SceneCaptureComponent->TextureTarget == nullptr) {return FPhotoData();}
    UTextureRenderTarget2D* DefaultRenderTarget = SceneCaptureComponent->TextureTarget;
    UTextureRenderTarget2D* TempRenderTarget = CopyRenderTarget(DefaultRenderTarget);

    SceneCaptureComponent->TextureTarget = TempRenderTarget;
    SceneCaptureComponent->CaptureScene();
    SceneCaptureComponent->TextureTarget = DefaultRenderTarget;

    FPhotoData PhotoData;
    if (PhotoDataCollector != nullptr && SceneCaptureComponent != nullptr)
    {
        FConvexVolume ViewFrustum = GetViewFrustum();
        FVector ViewLocation = SceneCaptureComponent->GetComponentLocation();
        PhotoData = PhotoDataCollector->CollectPhotoData(ViewFrustum, ViewLocation, GetActorForwardVector());
    }

    PhotoData.Image = TempRenderTarget;
    if (!PhotoData.Image->IsRooted()) {PhotoData.Image->AddToRoot();}

    return PhotoData;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraLens.h"
#include "ProjectCalm/Photos/PhotoDataCollectorComponent.h"
#include "ProjectCalm/Photos/PhotoData.h"
#include "ProjectCalm/Characters/Player/InfoFlagNameDefinitions.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/GameUserSettings.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Kismet/KismetRenderingLibrary.h"


ACameraLens::ACameraLens()
{
    PrimaryActorTick.bCanEverTick = true;

	PhotoDataCollector = CreateDefaultSubobject<UPhotoDataCollectorComponent>(TEXT("PhotoDataCollectorComponent"));
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(EquipmentMesh);
    SceneCaptureComponent->SetRelativeLocation(FVector(0.0f, 2.0f, 0.0f));
    SceneCaptureComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void ACameraLens::Equip(AActor* OwningActor, FName SocketName)
{
    Super::Equip(OwningActor, SocketName);    
    
    TargetFOV = MaxFOV;
    
    PlayerCameraComponent = GetPlayerCamera();

    if (SceneCaptureComponent == nullptr) {return;}
    SceneCaptureComponent->FOVAngle = TargetFOV;

    // Set TextureTarget size based on Screen Resolution    
    FIntPoint ScreenResolution = FIntPoint(1280, 720);
    if (GEngine) {ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();}    
    if (SceneCaptureComponent->TextureTarget != nullptr)
    {
        SceneCaptureComponent->TextureTarget->InitAutoFormat(ScreenResolution.X, ScreenResolution.Y);
        SceneCaptureComponent->TextureTarget->UpdateResourceImmediate();
    }
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

ULocalPlayer *ACameraLens::GetLocalPlayer()
{
    return GetWorld()->GetFirstLocalPlayerFromController();
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
    if (Viewport == nullptr) {return FConvexVolume();}
    
    FSceneInterface* SceneInterface = GetWorld()->Scene;
    FSceneViewFamilyContext ViewFamilyContext = FSceneViewFamily::ConstructionValues(Viewport, SceneInterface, ViewportClient->EngineShowFlags).SetRealtimeUpdate(true);
    
    FVector ViewLocation;
    FRotator ViewRotation;
    FSceneView* SceneView = LocalPlayer->CalcSceneView(&ViewFamilyContext, ViewLocation, ViewRotation, Viewport);
    if (SceneView == nullptr) {return FConvexVolume();}

    return SceneView->ViewFrustum;
}

void ACameraLens::SetupPlayerControls()
{
    Super::SetupPlayerControls();    

	if (LensZoomAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
		{
			EnhancedInputComponent->BindAction(LensZoomAction, ETriggerEvent::Triggered, this, &ACameraLens::ZoomAction);
		}
	}
}

void ACameraLens::ZoomAction(const FInputActionValue& Value)
{
    // Only alter FOV when camera is fully raised
    if (!GetPlayerFlag(FLAG_CAMERA_VIEW_ACTIVE)) {return;}

    TargetFOV -= Value.Get<float>() * GetWorld()->GetDeltaSeconds() * ZoomRate;
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
        PhotoData = PhotoDataCollector->CollectPhotoData(ViewFrustum, ViewLocation);
    }
    PhotoData.Image = TempRenderTarget;

    return PhotoData;
}

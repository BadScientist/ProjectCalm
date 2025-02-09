// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewBlenderComponent.h"
#include "PlayerCharacter.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/GameUserSettings.h"

#define BLEND_ALPHA_PARAM TEXT("BlendAlpha")

#ifdef PC_DEBUG_LOGS
	#define LOCAL_DEBUG_LOGS
#endif


// Sets default values for this component's properties
UViewBlenderComponent::UViewBlenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UViewBlenderComponent::BeginPlay()
{
	Super::BeginPlay();

	UWorld* World = GetWorld();
	if (BlendParametersAsset != nullptr && World != nullptr)
	{
		BlendParameters = World->GetParameterCollectionInstance(BlendParametersAsset);
	}
}

void UViewBlenderComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	SetEyePOV();

	UpdateCurrentBlendAlpha(DeltaTime);

	if (FMath::IsNearlyZero(TargetBlendAlpha - CurrentBlendAlpha)) {EndBlend();}

}

void UViewBlenderComponent::SetCharacterEyes(UCameraComponent *CameraComp)
{
	CharacterEyes = CameraComp;
	DefaultEyesFOV = CameraComp->FieldOfView;
}

float UViewBlenderComponent::BlendToNewView(USceneCaptureComponent2D *SceneCaptureComponent)
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Display, TEXT("%s: Blending to Camera View"), *GetName());
#endif // DEBUG

	TargetSceneCaptureComponent = SceneCaptureComponent;

	if (TargetSceneCaptureComponent != nullptr && GEngine != nullptr)
	{
		FIntPoint ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		TargetSceneCaptureComponent->TextureTarget->InitAutoFormat(FMath::Min(ScreenResolution.X, 1280), FMath::Min(ScreenResolution.Y, 720));
		TargetSceneCaptureComponent->TextureTarget->UpdateResourceImmediate();
	}

	return StartBlend(1.0f);
}

float UViewBlenderComponent::BlendToDefaultView()
{
	return StartBlend(0.0f);
}

void UViewBlenderComponent::SetEyePOV()
{
	// AFTER blending post process mat IN, set view to match SceneCaptureComponent
	if (bSetViewToLensNextFrame) {SetViewToSceneCapture(TargetSceneCaptureComponent);}

	// BEFORE blending post process mat OUT, reset view to default
	if (bResetViewNextFrame) {ResetView();}
}

void UViewBlenderComponent::SetViewToSceneCapture(USceneCaptureComponent2D* SceneCaptureComponent)
{
	CHECK_NULLPTR_RET(CharacterEyes, LogActorComponent, "ViewBlender: NO CHARACTER EYE CAMERA");
	CHECK_NULLPTR_RET(SceneCaptureComponent, LogActorComponent, "ViewBlender: NO SCENE CAPTURE COMPONENT");

#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Display, TEXT("%s: Setting view to scene capture component..."), *GetName());
#endif // DEBUG
	
	FTransform NewTransform = SceneCaptureComponent->GetComponentTransform();
	FTransform EyesTransform = CharacterEyes->GetComponentTransform();
	FTransform TransformOffset = NewTransform.GetRelativeTransform(EyesTransform);	
	CharacterEyes->AddAdditiveOffset(TransformOffset, 0);

	CharacterEyes->FieldOfView = SceneCaptureComponent->FOVAngle;
	
	bDefaultView = false;
	bSetViewToLensNextFrame = false;
}

void UViewBlenderComponent::ResetView()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Display, TEXT("%s: Resetting view..."), *GetName());
#endif // DEBUG

	if (CharacterEyes == nullptr) {return;}

	CharacterEyes->ClearAdditiveOffset();
	CharacterEyes->FieldOfView = DefaultEyesFOV;
	bDefaultView = true;
	bResetViewNextFrame = false;
}

float UViewBlenderComponent::GetScalarBlendParam()
{
	if (BlendParameters == nullptr) {return 0.0f;}
	float ParamValue;
    BlendParameters->GetScalarParameterValue(BLEND_ALPHA_PARAM, ParamValue);

	return ParamValue;
}

void UViewBlenderComponent::SetScalarBlendParam(float Alpha)
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Display, TEXT("%s: Setting blend alpha: %f"), *GetName(), Alpha);
#endif // DEBUG

	if (BlendParameters == nullptr) {return;}
	BlendParameters->SetScalarParameterValue(BLEND_ALPHA_PARAM, Alpha);
}

float UViewBlenderComponent::StartBlend(float TargetAlpha)
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogActorComponent, Display, TEXT("%s: Blending to alpha: %f"), *GetName(), TargetAlpha);
#endif // DEBUG

	if (bBlending && FMath::IsNearlyEqual(TargetAlpha, TargetBlendAlpha)) {return ViewBlendTime;}

	TargetBlendAlpha = TargetAlpha;
	ViewBlendTime = DefaultBlendTime * FMath::Abs(TargetAlpha - CurrentBlendAlpha);

	if (TargetSceneCaptureComponent != nullptr)
	{
		// TargetSceneCaptureComponent->CaptureScene();  // Do I need this??
		TargetSceneCaptureComponent->bCaptureEveryFrame = true;
	}

	if (FMath::IsNearlyZero(TargetAlpha))
	{
		SetScalarBlendParam(1.0f);
		bResetViewNextFrame = true;
	}
	
	bBlending = true;

	return ViewBlendTime;
}

void UViewBlenderComponent::UpdateCurrentBlendAlpha(float DeltaTime)
{
	if (!bBlending) {return;}

	ViewBlendTime -= DeltaTime;
	float NewBlendAlpha = FMath::Clamp(FMath::Square(DefaultBlendTime - ViewBlendTime) / FMath::Square(DefaultBlendTime), 0.0f, 1.0f);
	
	if (FMath::IsNearlyZero(TargetBlendAlpha)) {NewBlendAlpha = 1 - NewBlendAlpha;}  // For blending OUT postprocess material
	CurrentBlendAlpha = NewBlendAlpha;

	SetScalarBlendParam(CurrentBlendAlpha);
}

void UViewBlenderComponent::EndBlend()
{
	if (!bBlending) {return;}

	if (TargetSceneCaptureComponent != nullptr && GEngine != nullptr)
	{	
		TargetSceneCaptureComponent->bCaptureEveryFrame = false;
		FIntPoint ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		TargetSceneCaptureComponent->TextureTarget->InitAutoFormat(ScreenResolution.X, ScreenResolution.Y);
	}

	ViewBlendTime = 0.0f;
	if (!FMath::IsNearlyZero(TargetBlendAlpha)){bSetViewToLensNextFrame = true;}
	SetScalarBlendParam(0.0f);
	bBlending = false;
}

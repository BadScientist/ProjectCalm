// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewBlenderComponent.h"
#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Engine/TextureRenderTarget2D.h"
#include "GameFramework/GameUserSettings.h"

#define BLEND_ALPHA_PARAM TEXT("BlendAlpha")

// Sets default values for this component's properties
UViewBlenderComponent::UViewBlenderComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UViewBlenderComponent::BeginPlay()
{
	Super::BeginPlay();

	if (BlendParametersAsset != nullptr)
	{
		BlendParameters = GetWorld()->GetParameterCollectionInstance(BlendParametersAsset);
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
	return StartBlend(1.0f, SceneCaptureComponent);
}

float UViewBlenderComponent::BlendToDefaultView()
{
	return StartBlend(0.0f);
}

void UViewBlenderComponent::SetEyePOV()
{
	// AFTER blending post process mat IN, set view to match SceneCaptureComponent
	if (!bBlending && bDefaultView && TargetBlendAlpha) {SetViewToSceneCapture(TargetSceneCaptureComponent);}

	// BEFORE blending post process mat OUT, reset view to default
	if (bBlending && !bDefaultView && !TargetBlendAlpha) {ResetView();}
}

void UViewBlenderComponent::SetViewToSceneCapture(USceneCaptureComponent2D* SceneCaptureComponent)
{
	UE_LOG(LogTemp, Warning, TEXT("ViewBlender: Setting View to SceneCapture."));
	if (CharacterEyes == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("ViewBlender: NO CHARACTER EYE CAMERA"));
		return;
	}
	if (SceneCaptureComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("ViewBlender: NO SCENECAPTURECOMPONENT"));
		return;
	}
	
	FTransform NewTransform = SceneCaptureComponent->GetComponentTransform();
	FTransform EyesTransform = CharacterEyes->GetComponentTransform();
	FTransform TransformOffset = NewTransform.GetRelativeTransform(EyesTransform);	
	CharacterEyes->AddAdditiveOffset(TransformOffset, 0);

	CharacterEyes->FieldOfView = SceneCaptureComponent->FOVAngle;
	
	bDefaultView = false;
}

void UViewBlenderComponent::ResetView()
{
	UE_LOG(LogTemp, Warning, TEXT("ViewBlender: Resetting View."));
	if (CharacterEyes == nullptr) {return;}

	CharacterEyes->ClearAdditiveOffset();
	CharacterEyes->FieldOfView = DefaultEyesFOV;
	bDefaultView = true;
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
	if (BlendParameters == nullptr) {return;}
	BlendParameters->SetScalarParameterValue(BLEND_ALPHA_PARAM, Alpha);
}

float UViewBlenderComponent::StartBlend(float TargetAlpha, USceneCaptureComponent2D* SceneCaptureComponent)
{
	TargetSceneCaptureComponent = SceneCaptureComponent;

	if (TargetSceneCaptureComponent != nullptr && GEngine != nullptr)
	{
		FIntPoint ScreenResolution = GEngine->GetGameUserSettings()->GetScreenResolution();
		TargetSceneCaptureComponent->TextureTarget->InitAutoFormat(FMath::Min(ScreenResolution.X, 1280), FMath::Min(ScreenResolution.Y, 720));
		TargetSceneCaptureComponent->TextureTarget->UpdateResourceImmediate();
	}

	return StartBlend(TargetAlpha);
}

float UViewBlenderComponent::StartBlend(float TargetAlpha)
{
	TargetBlendAlpha = TargetAlpha;
	ViewBlendTime = DefaultBlendTime - ViewBlendTime;

	if (TargetSceneCaptureComponent != nullptr)
	{
		TargetSceneCaptureComponent->CaptureScene();
		TargetSceneCaptureComponent->bCaptureEveryFrame = true;
	}

	if (!TargetAlpha) {SetScalarBlendParam(1.0f);}
	bBlending = true;

	return ViewBlendTime;
}

void UViewBlenderComponent::UpdateCurrentBlendAlpha(float DeltaTime)
{
	if (!bBlending) {return;}

	ViewBlendTime -= DeltaTime;
	float NewBlendAlpha = FMath::Clamp(FMath::Square(DefaultBlendTime - ViewBlendTime) / FMath::Square(DefaultBlendTime), 0.0f, 1.0f);
	
	if (!TargetBlendAlpha) {NewBlendAlpha = 1 - NewBlendAlpha;}  // For blending OUT postprocess material
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
	SetScalarBlendParam(0.0f);	
	bBlending = false;
}

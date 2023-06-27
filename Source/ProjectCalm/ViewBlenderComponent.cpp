// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewBlenderComponent.h"
#include "PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "Materials/MaterialParameterCollectionInstance.h"

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
	if (!bBlending) {return;}

	UpdateCurrentBlendAlpha();

	if (FMath::IsNearlyZero(TargetBlendAlpha - CurrentBlendAlpha)) {EndBlend();}

}

void UViewBlenderComponent::BlendToNewView(USceneCaptureComponent2D* SceneCaptureComponent)
{
	if (CharacterEyes != nullptr) {SceneCaptureComponent->FOVAngle = CharacterEyes->FieldOfView;}
	StartBlend(1.0f, SceneCaptureComponent);
}

void UViewBlenderComponent::BlendToDefaultView()
{
	StartBlend(0.0f);
}

void UViewBlenderComponent::SetViewOffset(FTransform NewTransform)
{
	if (CharacterEyes == nullptr) {return;}

	FTransform EyesTransform = CharacterEyes->GetComponentTransform();
	FTransform TransformOffset = NewTransform.GetRelativeTransform(EyesTransform);
	CharacterEyes->AddAdditiveOffset(TransformOffset, 0);
}

void UViewBlenderComponent::ResetViewOffset()
{
	if (CharacterEyes == nullptr) {return;}

	CharacterEyes->ClearAdditiveOffset();
}

void UViewBlenderComponent::StartBlend(float TargetAlpha, USceneCaptureComponent2D* SceneCaptureComponent)
{
	TargetSceneCaptureComponent = SceneCaptureComponent;
	StartBlend(TargetAlpha);
}

void UViewBlenderComponent::StartBlend(float TargetAlpha)
{	
	if (BlendParameters == nullptr) {return;}

	TargetBlendAlpha = TargetAlpha;
	BlendStartTime = GetWorld()->GetTimeSeconds();

	if (!TargetAlpha)
	{
		BlendParameters->SetScalarParameterValue(BLEND_ALPHA_PARAM, 1.0f);
		ResetViewOffset();
	}

	bBlending = true;
}

void UViewBlenderComponent::UpdateCurrentBlendAlpha()
{
	if (BlendParameters == nullptr) {return;}

	float CurrentTime = GetWorld()->GetTimeSeconds();
	float NewBlendAlpha = FMath::Clamp(FMath::Square(CurrentTime - BlendStartTime) / FMath::Square(ViewBlendTime), 0.0f, 1.0f);
	
	if (!TargetBlendAlpha) {NewBlendAlpha = 1 - NewBlendAlpha;}  // For blending OUT postprocess material
	CurrentBlendAlpha = NewBlendAlpha;

	BlendParameters->SetScalarParameterValue(BLEND_ALPHA_PARAM, CurrentBlendAlpha);
}

void UViewBlenderComponent::EndBlend()
{
	// AFTER blending postprocess material IN, move view to SceneCaptureComponent location
	if (TargetBlendAlpha && TargetSceneCaptureComponent != nullptr) {SetViewOffset(TargetSceneCaptureComponent->GetComponentTransform());}

	if (BlendParameters != nullptr) {BlendParameters->SetScalarParameterValue(BLEND_ALPHA_PARAM, 0.0f);}
	
	bBlending = false;
}

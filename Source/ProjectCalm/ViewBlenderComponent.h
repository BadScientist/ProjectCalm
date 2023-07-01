// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ViewBlenderComponent.generated.h"

class APlayerCharacter;
class UCameraComponent;
class USceneCaptureComponent2D;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UViewBlenderComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UViewBlenderComponent();
	
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ViewBlender, meta = (AllowPrivateAccess = "true"))
	UMaterialParameterCollection* BlendParametersAsset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ViewBlender, meta = (AllowPrivateAccess = "true"))
	float DefaultBlendTime = 0.25f;

	UMaterialParameterCollectionInstance* BlendParameters;
	UCameraComponent* CharacterEyes;
	USceneCaptureComponent2D* TargetSceneCaptureComponent;
	float ViewBlendTime = 0.0f;

public:
	void SetCharacterEyes(UCameraComponent* CameraComp);
	float BlendToNewView(USceneCaptureComponent2D* SceneCaptureComponent);
	float BlendToDefaultView();

private:
	float DefaultEyesFOV = 90;
	float TargetBlendAlpha = 0.0f;
	float CurrentBlendAlpha = 0.0f;
	float BlendStartTime = 0.0f;
	bool bBlending = false;
	bool bDefaultView = true;

	void SetEyePOV();
	void SetViewToSceneCapture(USceneCaptureComponent2D* SceneCaptureComponent);
	void ResetView();
	float GetScalarBlendParam();
	void SetScalarBlendParam(float Alpha);
	
	float StartBlend(float TargetAlpha, USceneCaptureComponent2D* SceneCaptureComponent);
	float StartBlend(float TargetAlpha);
	void UpdateCurrentBlendAlpha(float DeltaTime);
	void EndBlend();
	
};

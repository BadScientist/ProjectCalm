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
	float ViewBlendTime = 0.0f;
	UMaterialParameterCollectionInstance* BlendParameters;
	UCameraComponent* CharacterEyes;
	USceneCaptureComponent2D* TargetSceneCaptureComponent;

public:
	void SetCharacterEyes(UCameraComponent* CameraComp) {CharacterEyes = CameraComp;};
	float GetViewBlendTime() {return ViewBlendTime;};
	void BlendToNewView(USceneCaptureComponent2D* SceneCaptureComponent);
	void BlendToDefaultView();

private:
	float TargetBlendAlpha = 0.0f;
	float CurrentBlendAlpha = 0.0f;
	bool bBlending = false;
	float BlendStartTime;

	void SetViewOffset(FTransform NewTransform);
	void ResetViewOffset();
	
	void StartBlend(float TargetAlpha, USceneCaptureComponent2D* SceneCaptureComponent);
	void StartBlend(float TargetAlpha);
	void UpdateCurrentBlendAlpha();
	void EndBlend();
	
};

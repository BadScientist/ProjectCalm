// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraLens.h"

#include "Camera/CameraComponent.h"
#include "Components/SceneCaptureComponent2D.h"
#include "EnhancedInputComponent.h"


ACameraLens::ACameraLens()
{
    PrimaryActorTick.bCanEverTick = true;
	
	SceneCaptureComponent = CreateDefaultSubobject<USceneCaptureComponent2D>(TEXT("SceneCaptureComponent"));
	SceneCaptureComponent->SetupAttachment(EquipmentMesh);
    SceneCaptureComponent->SetRelativeLocation(FVector(0.0f, 2.0f, 0.0f));
    SceneCaptureComponent->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));
}

void ACameraLens::BeginPlay()
{
    Super::BeginPlay();
    
    TargetFOV = MaxFOV;
}

void ACameraLens::Tick(float DeltaSeconds)
{
    if (UCameraComponent* CameraComponent = GetPlayerCamera())
    {
        CameraComponent->SetFieldOfView(TargetFOV);
    }
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
    TargetFOV += Value.Get<float>() * GetWorld()->GetDeltaSeconds() * ZoomRate;
    TargetFOV = FMath::Clamp(TargetFOV, MinFOV, MaxFOV);
}

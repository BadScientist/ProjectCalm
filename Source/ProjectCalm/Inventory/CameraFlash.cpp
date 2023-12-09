// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFlash.h"
#include "Components/SpotLightComponent.h"


ACameraFlash::ACameraFlash()
{
    PrimaryActorTick.bCanEverTick = true;
	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLight->SetupAttachment(EquipmentMesh, TEXT("SpotLightPoint"));
    SpotLight->SetRelativeLocation(FVector(0.0f, 5.0f, 1.25f));
    SpotLight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
}

void ACameraFlash::BeginPlay()
{
    Super::BeginPlay();
    if (SpotLight != nullptr) {SpotLight->SetVisibility(false);}
}

float ACameraFlash::PlayCameraFlash()
{
    if (SpotLight == nullptr) {return 0.0f;}

    SpotLight->SetVisibility(true);
    GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &ACameraFlash::DeactivateFlash, FlashDuration);

    return FlashDuration;
}

void ACameraFlash::DeactivateFlash()
{
    if (SpotLight == nullptr) {return;}

    SpotLight->SetVisibility(false);
}

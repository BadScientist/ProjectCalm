// Fill out your copyright notice in the Description page of Project Settings.


#include "CameraFlash.h"
#include "MeshSockets.h"
#include "EquipReply.h"
#include "ProjectCalm/Characters/Player/PlayerCharacter.h"

#include "Components/SpotLightComponent.h"


ACameraFlash::ACameraFlash()
{
    PrimaryActorTick.bCanEverTick = true;

    TargetSocket = SOCKET_CAMERA_FLASH;

	SpotLight = CreateDefaultSubobject<USpotLightComponent>(TEXT("SpotLightComponent"));
	SpotLight->SetupAttachment(GetEquipmentMesh(), TEXT("SpotLightPoint"));
    SpotLight->SetRelativeLocation(FVector(0.0f, 5.0f, 1.25f));
    SpotLight->SetRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
}

void ACameraFlash::BeginPlay()
{
    Super::BeginPlay();
    if (SpotLight != nullptr) {SpotLight->SetVisibility(false);}
}

EEquipReply ACameraFlash::Equip(APlayerCharacter *OwningCharacter)
{
    AActor* EquippedItem = Cast<AActor>(OwningCharacter->GetEquippedItem()->_getUObject());
    OwningPlayerCharacter = OwningCharacter;
    return Super::Equip_Internal(EquippedItem);
}

EEquipReply ACameraFlash::Equip_Internal(AActor* OwningActor)
{
    EEquipReply Response = Super::Equip_Internal(OwningActor);
    if (Response != EEquipReply::SUCCESS) {return EEquipReply::FAILED_NO_CAMERA;}
    return Response;
}

float ACameraFlash::PlayCameraFlash()
{
    if (SpotLight == nullptr) {return 0.0f;}

    SpotLight->SetVisibility(true);
    GetWorldTimerManager().SetTimer(FlashTimerHandle, this, &ACameraFlash::DeactivateFlash, FlashDuration);
    PlayPrimaryActionSound();

    return FlashDuration;
}

void ACameraFlash::DeactivateFlash()
{
    if (SpotLight == nullptr) {return;}

    SpotLight->SetVisibility(false);
}

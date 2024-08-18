// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"
#include "EquipReply.h"
#include "ProjectCalm/Characters/Player/InfoFlagNameDefinitions.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "InputActionValue.h"


ACompass::ACompass()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ACompass::SecondaryAction(const FInputActionValue &Value)
{
	if (OwningPlayerCharacter != nullptr) {OwningPlayerCharacter->RestrictMovement(Value.Get<bool>());}

    SetPlayerFlag(FLAG_EQUIPMENT_RAISED, Value.Get<bool>());
}

void ACompass::BeginPlay()
{
	Super::BeginPlay();
	
	AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this);
	CHECK_NULLPTR_RET(GameMode, LogEquipment, "Compass:: No ProjectCalmGameMode found!");

	SetNeedleTargetLocation(GameMode->GetSpecialRockLocation());
	GameMode->OnSpecialRockMoved.AddDynamic(this, &ACompass::SetNeedleTargetLocation);
}

EEquipReply ACompass::Equip_Internal(AActor* OwningActor)
{
	EEquipReply Response = Super::Equip_Internal(OwningActor);

	if (Response == EEquipReply::SUCCESS) {SetPlayerFlag(FLAG_PLAYER_HAS_COMPASS, true);}

	return Response;
}

void ACompass::Unequip()
{
	SetPlayerFlag(FLAG_PLAYER_HAS_COMPASS, false);

	Super::Unequip();
}

void ACompass::SetNeedleTargetAngle()
{
	CHECK_NULLPTR_RET(OwningPlayerCharacter, LogEquipment, "Compass:: No PlayerCharacter found!");

	FVector2D NeedleTarget = FVector2D(NeedleTargetLocation);
	FVector2D ActorLocation = FVector2D(GetActorLocation());

	FVector2D NeedleDirection = (NeedleTarget - ActorLocation).GetSafeNormal();
	FVector2D PlayerForward = FVector2D(OwningPlayerCharacter->GetActorForwardVector()).GetSafeNormal();
	FVector2D PlayerRight = FVector2D(OwningPlayerCharacter->GetActorRightVector()).GetSafeNormal();

	float RotationAngle = FMath::RadiansToDegrees(FMath::Acos(NeedleDirection.Dot(PlayerForward)));
	if (NeedleDirection.Dot(PlayerRight) < 0) {RotationAngle = 360 - RotationAngle;}

    NeedleTargetAngle = RotationAngle;
}

void ACompass::SetCurrentNeedleAngle(const float InAngle)
{
	if (InAngle < 0) {CurrentNeedleAngle = 360 + InAngle;}
	else if (InAngle >= 360) {CurrentNeedleAngle = InAngle - 360;}
	else {CurrentNeedleAngle = InAngle;}
}

void ACompass::SetCurrentRotationRate(const float InRate)
{
	if (InRate > MaxNeedleRotationRate) {CurrentRotationRate = MaxNeedleRotationRate;}
	else if (InRate < -1 * MaxNeedleRotationRate) {CurrentRotationRate = -1 * MaxNeedleRotationRate;}
	else {CurrentRotationRate = InRate;}
}

float ACompass::CalculateOffsetAngle() const
{
	float ClockwiseOffset = NeedleTargetAngle > CurrentNeedleAngle ?
		NeedleTargetAngle - CurrentNeedleAngle :
		NeedleTargetAngle + (360 - CurrentNeedleAngle);
	float CounterClockwiseOffset = NeedleTargetAngle > CurrentNeedleAngle ?
		NeedleTargetAngle - 360 - CurrentNeedleAngle :
		NeedleTargetAngle - CurrentNeedleAngle;
    return FMath::Abs(ClockwiseOffset) <= FMath::Abs(CounterClockwiseOffset) ? ClockwiseOffset : CounterClockwiseOffset;
}

void ACompass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetNeedleTargetAngle();

	float OffsetAngle = CalculateOffsetAngle();

	float LerpAlpha = FMath::Clamp(FMath::Abs(OffsetAngle) / MaxAccelerationAngleOffset, 0, 1);
	float Acceleration = FMath::Lerp(0, MaxNeedleAcceleration, LerpAlpha) * FMath::Sign(OffsetAngle);

	SetCurrentRotationRate(CurrentRotationRate + Acceleration);
	SetCurrentNeedleAngle(CurrentNeedleAngle + CurrentRotationRate * DeltaTime);
}

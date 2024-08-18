// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"
#include "Compass.generated.h"


/**
 * 
 */
UCLASS()
class PROJECTCALM_API ACompass : public AEquipment
{
	GENERATED_BODY()

	// Degrees/sec/sec
	UPROPERTY(EditDefaultsOnly, Category = Tracking)
	float MaxNeedleAcceleration{5.0f};

	// Degrees/sec
	UPROPERTY(EditDefaultsOnly, Category = Tracking)
	float MaxNeedleRotationRate{15.0f};

	UPROPERTY(EditDefaultsOnly, Category = Tracking)
	float MaxAccelerationAngleOffset{30.0f};
	
	UPROPERTY(VisibleAnywhere, Category = Tracking, BlueprintGetter = GetNeedleTargetLocation, BlueprintSetter = SetNeedleTargetLocation, meta = (AllowPrivateAccess = "true"))
	FVector NeedleTargetLocation{FVector::ZeroVector};
	
	UPROPERTY(VisibleAnywhere, Category = Tracking, BlueprintGetter = GetNeedleTargetAngle, meta = (AllowPrivateAccess = "true"))
	float NeedleTargetAngle{0.0f};

	UPROPERTY(VisibleAnywhere, Category = Tracking, BlueprintGetter = GetCurrentNeedleAngle, BlueprintSetter = SetCurrentNeedleAngle, meta = (AllowPrivateAccess = "true"))
	float CurrentNeedleAngle{0.0f};

	UPROPERTY(VisibleAnywhere, Category = Tracking, BlueprintGetter = GetCurrentRotationRate, BlueprintSetter = SetCurrentRotationRate, meta = (AllowPrivateAccess = "true"))
	float CurrentRotationRate{0.0f};

public:
	ACompass();

	void SecondaryAction(const FInputActionValue& Value) override;

protected:
	virtual void BeginPlay() override;
	
	virtual EEquipReply Equip_Internal(AActor* OwningActor) override;
	
	/* START EQUIPMENT INTERFACE */
	virtual void Unequip() override;
	/* END EQUIPMENT INTERFACE */

public:
	UFUNCTION(BlueprintPure)
	FVector GetNeedleTargetLocation() const {return NeedleTargetLocation;};

	UFUNCTION(BlueprintCallable)
	void SetNeedleTargetLocation(const FVector InLocation) {NeedleTargetLocation = InLocation;};

	UFUNCTION(BlueprintPure)
	float GetNeedleTargetAngle() const {return NeedleTargetAngle;};

	UFUNCTION(BlueprintCallable)
	void SetNeedleTargetAngle();

	UFUNCTION(BlueprintPure)
	float GetCurrentNeedleAngle() const {return CurrentNeedleAngle;};

	UFUNCTION(BlueprintCallable)
	void SetCurrentNeedleAngle(const float InAngle);

	UFUNCTION(BlueprintPure)
	float GetCurrentRotationRate() const {return CurrentRotationRate;};

	UFUNCTION(BlueprintCallable)
	void SetCurrentRotationRate(const float InRate);	

	UFUNCTION(BlueprintPure)
	float CalculateOffsetAngle() const;

	virtual void Tick(float DeltaTime) override;
	
};

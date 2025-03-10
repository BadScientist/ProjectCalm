// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "EquipmentInterface.h"
#include "Equipment.generated.h"

class UCameraComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UEnhancedInputComponent;
class UInputAction;
class UInputMappingContext;
class IEquipmentInterface;
class APlayerCharacter;
class USoundCue;
struct FInputActionValue;
struct FInfoFlag;
struct FEnhancedInputActionEventBinding;
enum EEquipReply : uint8;


UCLASS()
class PROJECTCALM_API AEquipment : public AActor, public IEquipmentInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* EquipmentMappingContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipmentPrimaryAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* EquipmentSecondaryAction;

    UPROPERTY(EditAnywhere, Category = SFX)
    FName PrimaryActionSound;

    UPROPERTY(EditAnywhere, Category = SFX)
    FName SecondaryActionSound;

	FEnhancedInputActionEventBinding* PrimaryInputStartBinding;
	FEnhancedInputActionEventBinding* PrimaryInputCompletedBinding;
	FEnhancedInputActionEventBinding* SecondaryInputStartBinding;
	FEnhancedInputActionEventBinding* SecondaryInputCompletedBinding;

	int32 InstanceID{0};
	
public:	
	// Sets default values for this actor's properties
	AEquipment();


protected:
	APlayerCharacter* OwningPlayerCharacter{nullptr};

	virtual EEquipReply Equip_Internal(AActor* OwningActor);

	// START EQUIPMENT INTERFACE
	virtual EEquipReply Equip(APlayerCharacter* OwningCharacter) override;
	virtual void Unequip() override;
	void SetInstanceID(int32 InID) {if (InstanceID == 0) {InstanceID = InID;}};
	int32 GetInstanceID() {return InstanceID;};
	// END EQUIPMENT INTERFACE

	virtual void SetupPlayerControls();
	virtual void ResetPlayerControls();
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* EquipmentMesh;
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* EquipmentSkeletalMesh;

	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float PrimaryActionCooldown = 0.1f;
	UPROPERTY(EditDefaultsOnly, Category = Input, meta = (ClampMin = "0.0", UIMin = "0.0"))
	float SecondaryActionCooldown = 0.f;

	bool bPrimaryActionOnCooldown = false;
	bool bSecondaryActionOnCooldown = false;
	FTimerHandle PrimaryActionCooldownTimerHandle;
	FTimerHandle SecondaryActionCooldownTimerHandle;
	void SetPrimaryActionOnCooldown() {bPrimaryActionOnCooldown = true;};
	void SetSecondaryActionOnCooldown() {bSecondaryActionOnCooldown = true;};
	void ClearPrimaryActionOnCooldown() {bPrimaryActionOnCooldown = false;};
	void ClearSecondaryActionOnCooldown() {bSecondaryActionOnCooldown = false;};

	void PlayPrimaryActionSound();
	void PlaySecondaryActionSound();

	bool GetPlayerFlag(FName FlagName);
	void SetPlayerFlag(FName FlagName, bool Value);

public:

	virtual void PrimaryAction(const FInputActionValue& Value) {};
	virtual void SecondaryAction(const FInputActionValue& Value) {};

	UMeshComponent* GetEquipmentMesh();

};

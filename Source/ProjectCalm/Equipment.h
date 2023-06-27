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
struct FInputActionValue;

USTRUCT()
struct FEquipmentFlag
{
	GENERATED_BODY();

	FEquipmentFlag(){};
	FEquipmentFlag(FName NewFlagName, bool InitialValue)
	{
		FlagName = NewFlagName;
		Value = InitialValue;
	};

	FName FlagName;
	bool Value;
};

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
	
public:	
	// Sets default values for this actor's properties
	AEquipment();

	virtual bool GetFlagByName(FName SearchName) override;
	virtual void SetFlag(FName SearchName, bool Value) override;

protected:
	virtual void Equip(AActor* OwningActor, FName SocketName) override;
	
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* EquipmentMesh;
	
	UPROPERTY(EditAnywhere, Category = Equipment)
	TArray<FEquipmentFlag> EquipmentFlags;

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

	APlayerCharacter* GetPlayerCharacter();
	APlayerController* GetPlayerController();
	UCameraComponent* GetPlayerCamera();
	UEnhancedInputComponent* GetEnhancedInputComponent();
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();
	virtual void SetupPlayerControls();

public:
	virtual void PrimaryAction(const FInputActionValue& Value) {};
	virtual void SecondaryAction(const FInputActionValue& Value) {};
	UStaticMeshComponent* GetEquipmentMesh() {return EquipmentMesh;};

};

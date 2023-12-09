// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USceneCaptureComponent2D;
class UViewBlenderComponent;
class UFlagManagerComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;
class IEquipmentInterface;
class USpawnerComponent;
class APhotoCameraEquipment;  // TODO: Add Menu System for adding equipment
class ACameraFlash;  // TODO: Add Menu System for adding equipment
class ACameraLens;  // TODO: Add Menu System for adding equipment

UCLASS(config = game)
class PROJECTCALM_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UViewBlenderComponent* ViewBlenderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flags, meta = (AllowPrivateAccess = "true"))
	UFlagManagerComponent* FlagManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flags, meta = (AllowPrivateAccess = "true"))
	USpawnerComponent* SpawnerComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;

	APlayerController* PlayerController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	// TODO: Add Menu System for adding equipment
	TSubclassOf<APhotoCameraEquipment> PhotoCameraClass;
	TSubclassOf<ACameraLens> CameraLensClass;
	TSubclassOf<ACameraFlash> CameraFlashClass;

public:
	// Default constructor
	APlayerCharacter();

protected:
	IEquipmentInterface* EquippedItem;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

public:
	UCameraComponent* GetCameraComponent() {return FirstPersonCamera;};
	float BlendViewToSceneCaptureComponent(USceneCaptureComponent2D* SceneCaptureComponent);
	float ResetCameraLocation();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintPure)
	bool GetFlag(FName FlagName) const;
	UFUNCTION(BlueprintCallable)
	void SetFlag(FName FlagName, bool Value);

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"

#include "ProjectCalm/Inventory/EquipperInterface.h"
#include "PlayerCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogPlayerCharacter, All, All)

class UCameraComponent;
class USceneCaptureComponent2D;
class UViewBlenderComponent;
class UFlagManagerComponent;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;
class IEquipmentInterface;
class USpawnerComponent;
class UInventoryComponent;
class UItemData;
enum EEquipReply;
class APhotoCameraEquipment;  // TODO: Add Menu System for adding equipment
class ACameraFlash;  // TODO: Add Menu System for adding equipment
class ACameraLens;  // TODO: Add Menu System for adding equipment


UCLASS(config = game)
class PROJECTCALM_API APlayerCharacter : public ACharacter, public IEquipperInterface
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
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;

	APlayerController* PlayerController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	bool bIsMenuOpen{false};

public:
	// Default constructor
	APlayerCharacter();

protected:
	IEquipmentInterface* EquippedItem;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Pause(const FInputActionValue& Value);
	void OpenInventory(const FInputActionValue& Value);

	// START IEQUIPPERINTERFACE
	virtual bool AttachEquipment(IEquipmentInterface* Equipment, FName SocketName) override;
	virtual void RemoveEquipment(IEquipmentInterface* Equipment) override {EquippedItem = nullptr;};
	// END IEQUIPPERINTERFACE

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void Jump() override;

	UCameraComponent* GetCameraComponent() {return FirstPersonCamera;};

	UInventoryComponent* GetInventoryComponent() {return InventoryComponent;};
	IEquipmentInterface* GetEquippedItem() {return EquippedItem;};
	void GetInventory(TArray<UItemData*>& OutInventory);
	void SwapInventoryItems(int32 FirstIndex, int32 SecondIndex);

	float BlendViewToSceneCaptureComponent(USceneCaptureComponent2D* SceneCaptureComponent);
	float ResetCameraLocation();

	UFUNCTION(BlueprintPure)
	bool GetFlag(FName FlagName) const;
	UFUNCTION(BlueprintCallable)
	void SetFlag(FName FlagName, bool Value);

};

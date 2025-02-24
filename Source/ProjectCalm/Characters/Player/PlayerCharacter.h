// Copyright 2025 Joseph D Tong aka "BadScientist"

#pragma once

#include "CoreMinimal.h"
#include "ProjectCalm/Characters/PausableCharacter.h"
#include "InputActionValue.h"

#include "ProjectCalm/Inventory/EquipperInterface.h"
#include "PlayerCharacter.generated.h"

class UCameraComponent;
class USpotLightComponent;
class USceneCaptureComponent2D;
class UInteractionComponent;
class UViewBlenderComponent;
class UFlagManagerComponent;
class UNotificationComponent;
class UNoiseMakerComponent;
class UPCPerceptionStimulusComponent;
class UPlayerHUD;
class UEnhancedInputLocalPlayerSubsystem;
class UInputAction;
class UInputMappingContext;
class IEquipmentInterface;
class USpawnerComponent;
class UHealthComponent;
class UInventoryComponent;
class UItemData;
enum EEquipReply : uint8;


UCLASS(config = game)
class PROJECTCALM_API APlayerCharacter : public APausableCharacter, public IEquipperInterface
{
	GENERATED_BODY()

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USceneComponent* ViewPivotComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCamera;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpotLightComponent* SpotLight;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UViewBlenderComponent* ViewBlenderComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Flags, meta = (AllowPrivateAccess = "true"))
	UFlagManagerComponent* FlagManagerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerNotifications, meta = (AllowPrivateAccess = "true"))
	UNotificationComponent* NotificationComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PhotoSubjectSpawns, meta = (AllowPrivateAccess = "true"))
	USpawnerComponent* SpawnerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Inventory, meta = (AllowPrivateAccess = "true"))
	UInventoryComponent* InventoryComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UNoiseMakerComponent* NoiseMakerComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UPCPerceptionStimulusComponent* StimulusSourceComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UInteractionComponent* InteractionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Gameplay, meta = (AllowPrivateAccess = "true"))
	UHealthComponent* HealthComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputMappingContext* DefaultMappingContext;	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* MoveAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* CrouchAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* SprintAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* PauseAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenInventoryAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenQuestLogAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* OpenPhotoLogAction;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = HUD)
	TSubclassOf<UPlayerHUD> PlayerHUDClass;
	UPlayerHUD* PlayerHUD{nullptr};

	APlayerController* PlayerController;
	UEnhancedInputLocalPlayerSubsystem* Subsystem;

	bool bIsMenuOpen{false};
	bool bIsMovementRestricted{false};

	FVector RespawnPoint{FVector::ZeroVector};

	FVector ViewAdjustmentLocation{FVector::ZeroVector};
	bool bIsCrouching{false};

public:
	// Default constructor
	APlayerCharacter(const FObjectInitializer& ObjectInitializer);

	virtual void Tick( float DeltaSeconds );

protected:
	IEquipmentInterface* EquippedItem;

	virtual void BeginPlay() override;

	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void StartCrouching();
	void StopCrouching();
	void StartSprinting();
	void StopSprinting();
	void Pause(const FInputActionValue& Value);
	void OpenInventory(const FInputActionValue& Value);
	void OpenQuestLog(const FInputActionValue& Value);
	void OpenPhotoLog(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);

	// START IEQUIPPERINTERFACE
	virtual bool AttachEquipment(IEquipmentInterface* Equipment, FName SocketName) override;
	virtual void RemoveEquipment(IEquipmentInterface* Equipment) override {EquippedItem = nullptr;};
	// END IEQUIPPERINTERFACE

	UFUNCTION()
	virtual void HandleDeath(FString DamageMessage);

	UFUNCTION()
	virtual void Respawn();

private:
	void DisplayDeathScreen(FString DamageMessage);
	void AdjustViewToHalfHeight(float DeltaTime);

public:
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;
	virtual void OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust) override;

	void SetRespawnPoint(FVector InVector) {RespawnPoint = InVector;};

	UCameraComponent* GetCameraComponent() {return FirstPersonCamera;};

	UInventoryComponent* GetInventoryComponent() {return InventoryComponent;};

	IEquipmentInterface* GetEquippedItem() {return EquippedItem;};

	UFUNCTION(BlueprintPure)
	UObject* GetEquippedItemObject() const;

	void GetInventory(TArray<UItemData*>& OutInventory);
	int32 GetInventorySlotsRemaining();
	bool AddItem(UItemData* Item);
	bool RemoveItem(UItemData* Item);
	void SwapInventoryItems(int32 FirstIndex, int32 SecondIndex);

	float BlendViewToSceneCaptureComponent(USceneCaptureComponent2D* SceneCaptureComponent);
	float ResetCameraLocation();

	UFUNCTION(BlueprintPure)
	bool GetFlag(FName FlagName) const;
	UFUNCTION(BlueprintCallable)
	void SetFlag(FName FlagName, bool Value);
	UFUNCTION(BlueprintCallable)
	void NotifyPlayer(FString InString);
	
	UFUNCTION()
	void ShowHUD();
	void HideHUD();

	// Disable Jumping and halve maximum walking speed
	void RestrictMovement(bool bValue);

};

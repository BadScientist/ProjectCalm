// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InteractionComponent.h"
#include "ViewBlenderComponent.h"
#include "FlagManagerComponent.h"
#include "InfoFlagNameDefinitions.h"
#include "NotificationComponent.h"
#include "SpawnerComponent.h"
#include "PlayerCharacterMovementComponent.h"
#include "ProjectCalm/Characters/HealthComponent.h"
#include "ProjectCalm/Gameplay/NoiseMakerComponent.h"
#include "ProjectCalm/Game/ProjectCalmGameInstance.h"
#include "ProjectCalm/Inventory/InventoryComponent.h"
#include "ProjectCalm/Inventory/EquipmentInterface.h"
#include "ProjectCalm/Inventory/MeshSockets.h"
#include "ProjectCalm/UI/PlayerHUD.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/AI/PCPerceptionStimulusComponent.h"

#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

#define EYE_VERTICAL_DEPTH 36.0f


// Sets default values
APlayerCharacter::APlayerCharacter(const FObjectInitializer& ObjectInitializer) :
	Super(ObjectInitializer.SetDefaultSubobjectClass<UPlayerCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	UCapsuleComponent* CapsuleComp = GetCapsuleComponent();
	CHECK_NULLPTR_RET(CapsuleComp, LogPlayerCharacter, "PlayerCharacter:: Failed to get Capsule Component!");

	CapsuleComp->InitCapsuleSize(55.f, 96.f);

	ViewPivotComponent = CreateDefaultSubobject<USceneComponent>(TEXT("ViewPivotComponent"));
	CHECK_NULLPTR_RET(ViewPivotComponent, LogPlayerCharacter, "PlayerCharacter:: Failed to create View Pivot Component!");

	ViewPivotComponent->SetupAttachment(CapsuleComp);
	float CapsuleHalfHeight = CapsuleComp->GetScaledCapsuleHalfHeight();
	ViewPivotComponent->SetRelativeLocation(FVector(0.0f, 0.0f, CapsuleHalfHeight - EYE_VERTICAL_DEPTH));
	
	// Setup player mesh
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	CHECK_NULLPTR_RET(CharacterMesh, LogPlayerCharacter, "PlayerCharacter:: Failed to get Skeletal Mesh Component!");

	CharacterMesh->SetupAttachment(ViewPivotComponent);
	CharacterMesh->SetOnlyOwnerSee(true);
	CharacterMesh->bCastDynamicShadow = false;
	CharacterMesh->CastShadow = false;
	CharacterMesh->SetRelativeLocation(FVector(15.0f, 0.0f, -2 * CapsuleHalfHeight + EYE_VERTICAL_DEPTH));
	CharacterMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));

	//Setup first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CHECK_NULLPTR_RET(FirstPersonCamera, LogPlayerCharacter, "PlayerCharacter:: Failed to create Camera Component!");	

	FirstPersonCamera->SetupAttachment(CharacterMesh, SOCKET_PLAYER_CAMERA);
	FirstPersonCamera->bUsePawnControlRotation = false;

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionComponent"));
	if (InteractionComponent != nullptr) {InteractionComponent->SetupAttachment(FirstPersonCamera);}
	
	// Setup Actor Components
	ViewBlenderComponent = CreateDefaultSubobject<UViewBlenderComponent>(TEXT("ViewBlenderComponent"));
	if (ViewBlenderComponent != nullptr) {ViewBlenderComponent->SetCharacterEyes(FirstPersonCamera);}
	
	FlagManagerComponent = CreateDefaultSubobject<UFlagManagerComponent>(TEXT("FlagManagerComponent"));
	NotificationComponent = CreateDefaultSubobject<UNotificationComponent>(TEXT("NotificationComponent"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	StimulusSourceComponent = CreateDefaultSubobject<UPCPerceptionStimulusComponent>(TEXT("StimulusSourceComponent"));
	

	SpawnerComponent = CreateDefaultSubobject<USpawnerComponent>(TEXT("SpawnerComponent"));
	if (SpawnerComponent != nullptr) {SpawnerComponent->SetupAttachment(RootComponent);}
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Add Input Mapping Context
	PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController != nullptr)
	{
		Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem != nullptr) {Subsystem->AddMappingContext(DefaultMappingContext, 0);}
	}

	if (PlayerHUDClass != nullptr) {PlayerHUD = CreateWidget<UPlayerHUD>(PlayerController, PlayerHUDClass.Get());}

	if (PlayerHUD != nullptr) 
	{
		PlayerHUD->AddToViewport();
		
		if (NotificationComponent != nullptr) {NotificationComponent->SetActiveWidget(PlayerHUD->GetNotificationWidget());}
		if (InteractionComponent != nullptr) {InteractionComponent->SetInteractionLabelText(PlayerHUD->GetInteractionLabelText());}
	}

	if (HealthComponent != nullptr) {HealthComponent->OnDeath.AddDynamic(this, &APlayerCharacter::HandleDeath);}

	if (AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this))
	{
		GameMode->OnPlayerRespawn.AddDynamic(this, &APlayerCharacter::Respawn);
	}

	SetRespawnPoint(GetActorLocation());

	if (ViewPivotComponent != nullptr) {ViewAdjustmentLocation = ViewPivotComponent->GetRelativeLocation();}
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	AdjustViewToHalfHeight(DeltaSeconds);
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartCrouching);
		EnhancedInputComponent->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouching);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Triggered, this, &APlayerCharacter::StartSprinting);
		EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprinting);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pause);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenInventory);
		EnhancedInputComponent->BindAction(OpenQuestLogAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenQuestLog);
		EnhancedInputComponent->BindAction(OpenPhotoLogAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenPhotoLog);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Interact);
	}
}

void APlayerCharacter::OnStartCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// RecalculateBaseEyeHeight();
	if (ViewPivotComponent)
	{
		ViewPivotComponent->SetRelativeLocation(ViewPivotComponent->GetRelativeLocation() + FVector(0.0f, 0.0f, 2 * HalfHeightAdjust - EYE_VERTICAL_DEPTH));
	}

	ViewAdjustmentLocation -= FVector(0.0f, 0.0f, HalfHeightAdjust);

	bIsCrouching = true;

	K2_OnStartCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

void APlayerCharacter::OnEndCrouch(float HalfHeightAdjust, float ScaledHalfHeightAdjust)
{
	// RecalculateBaseEyeHeight();
	if (ViewPivotComponent) {ViewPivotComponent->SetRelativeLocation(ViewPivotComponent->GetRelativeLocation() - FVector(0.0f, 0.0f, HalfHeightAdjust));}

	ViewAdjustmentLocation += FVector(0.0f, 0.0f, HalfHeightAdjust);

	bIsCrouching = false;

	K2_OnEndCrouch(HalfHeightAdjust, ScaledHalfHeightAdjust);
}

bool APlayerCharacter::GetFlag(FName FlagName) const
{
	CHECK_NULLPTR_RETVAL(FlagManagerComponent, LogPlayerCharacter, "PlayerCharacter:: No FlagManagerComponent!", false);
	return FlagManagerComponent->GetFlag(FlagName);
}

void APlayerCharacter::SetFlag(FName FlagName, bool Value)
{
	CHECK_NULLPTR_RET(FlagManagerComponent, LogPlayerCharacter, "PlayerCharacter:: No FlagManagerComponent!");
	FlagManagerComponent->SetFlag(FlagName, Value);
}

void APlayerCharacter::NotifyPlayer(FString InString)
{
	CHECK_NULLPTR_RET(NotificationComponent, LogPlayerCharacter, "PlayerCharacter:: NotificationComponent not found!");
	NotificationComponent->DisplayString(InString);
}

void APlayerCharacter::ShowHUD()
{
	if (PlayerHUD != nullptr) {PlayerHUD->Show();}
}

void APlayerCharacter::HideHUD()
{
	if (PlayerHUD != nullptr) {PlayerHUD->Hide();}
}

void APlayerCharacter::RestrictMovement(bool bValue)
{
	UPlayerCharacterMovementComponent* MovementComp = Cast<UPlayerCharacterMovementComponent>(GetMovementComponent());
	CHECK_NULLPTR_RET(MovementComp, LogPlayerCharacter, "PlayerCharacter:: No Movement Component!");
	MovementComp->SetWantsToRestrict(bValue);
}

void APlayerCharacter::Move(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	CHECK_NULLPTR_RET(Controller, LogPlayerCharacter, "PlayerCharacter:: No Controller!");

	FVector2D Direction = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Direction.Y);
	AddMovementInput(GetActorRightVector(), Direction.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	CHECK_NULLPTR_RET(Controller, LogPlayerCharacter, "PlayerCharacter:: No Controller!");

	FVector2D Direction = Value.Get<FVector2D>();
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);

	const FRotator ViewRotation = GetViewRotation();
	const FRotator NewViewRotation = FRotator(ViewRotation.Pitch, ViewRotation.Yaw, 0.0f);
	if (ViewPivotComponent != nullptr && !NewViewRotation.Equals(ViewPivotComponent->GetComponentRotation()))
	{
		ViewPivotComponent->SetWorldRotation(NewViewRotation);
	}
}

void APlayerCharacter::StartCrouching()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerCharacterMovementComponent:: StartCrouching"));
#endif

	Crouch(false);
}

void APlayerCharacter::StopCrouching()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerCharacterMovementComponent:: StopCrouching"));
#endif

	UnCrouch(false);
}

void APlayerCharacter::StartSprinting()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerCharacterMovementComponent:: StartSprinting"));
#endif

	UPlayerCharacterMovementComponent* MovementComp = Cast<UPlayerCharacterMovementComponent>(GetMovementComponent());
	CHECK_NULLPTR_RET(MovementComp, LogPlayerCharacter, "PlayerCharacter:: No Movement Component!");
	MovementComp->SetWantsToSprint(true);
}

void APlayerCharacter::StopSprinting()
{
#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogPlayerCharacter, Warning, TEXT("PlayerCharacterMovementComponent:: StopSprinting"));
#endif

	UPlayerCharacterMovementComponent* MovementComp = Cast<UPlayerCharacterMovementComponent>(GetMovementComponent());
	CHECK_NULLPTR_RET(MovementComp, LogPlayerCharacter, "PlayerCharacter:: No Movement Component!");
	MovementComp->SetWantsToSprint(false);
}

void APlayerCharacter::Pause(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: Could not get ProjectCalmGameInstance!");
	GameInstance->LoadPauseMenu();
}

void APlayerCharacter::OpenInventory(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: Could not get ProjectCalmGameInstance!");
	GameInstance->LoadInventoryMenu();
}

void APlayerCharacter::OpenQuestLog(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: Could not get ProjectCalmGameInstance!");
	GameInstance->LoadQuestLog();
}

void APlayerCharacter::OpenPhotoLog(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: Could not get ProjectCalmGameInstance!");
	GameInstance->LoadPhotoLog(EquippedItem != nullptr ? EquippedItem->GetInstanceID() : 0);
}

void APlayerCharacter::Interact(const FInputActionValue &Value)
{
	CHECK_NULLPTR_RET(InteractionComponent, LogPlayerCharacter, "PlayerCharacter:: No InteractionComponent found!")
	InteractionComponent->Interact();
}

bool APlayerCharacter::AttachEquipment(IEquipmentInterface *Equipment, FName SocketName)
{
	if (SocketName != SOCKET_PLAYER_GRIP) {return false;}
	if (AActor* EquipmentActor = Cast<AActor>(Equipment->_getUObject()))
	{
		EquipmentActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		EquippedItem = Equipment;
		return true;
	}
	return false;
}

void APlayerCharacter::HandleDeath(FString DamageMessage)
{
	SetFlag(FLAG_PLAYER_IS_DEAD, true);
	DisplayDeathScreen(DamageMessage);
}

void APlayerCharacter::Respawn()
{
	SetFlag(FLAG_PLAYER_IS_DEAD, false);
	TeleportTo(RespawnPoint, FRotator::ZeroRotator);
	if (HealthComponent != nullptr) {HealthComponent->Reset();}
}

void APlayerCharacter::DisplayDeathScreen(FString DamageMessage)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: Could not get ProjectCalmGameInstance!");
	GameInstance->LoadDeathScreen(DamageMessage);
}

void APlayerCharacter::AdjustViewToHalfHeight(float DeltaTime)
{
	CHECK_NULLPTR_RET(ViewPivotComponent, LogPlayerCharacter, "PlayerCharacter:: No ViewPivotComponent!");

	FVector CurrentLocation = ViewPivotComponent->GetRelativeLocation();
	if (CurrentLocation == ViewAdjustmentLocation) {return;}

	if (bIsCrouching && (CurrentLocation.Z < ViewAdjustmentLocation.Z || FMath::IsNearlyEqual(CurrentLocation.Z, ViewAdjustmentLocation.Z)) \
	|| !bIsCrouching && (CurrentLocation.Z > ViewAdjustmentLocation.Z || FMath::IsNearlyEqual(CurrentLocation.Z, ViewAdjustmentLocation.Z)))
	{
		ViewPivotComponent->SetRelativeLocation(ViewAdjustmentLocation);
		return;
	}

	if (ViewPivotComponent)
	{
		float Adjustment = 500.0f * DeltaTime * (bIsCrouching ? -1.0f : 1.0f);
		ViewPivotComponent->SetRelativeLocation(CurrentLocation + FVector(0.0f, 0.0f, Adjustment));
	}
}

void APlayerCharacter::GetInventory(TArray<UItemData*>& OutInventory)
{
	CHECK_NULLPTR_RET(InventoryComponent, LogPlayerCharacter, "PlayerCharacter:: No InventoryComponent!");
	InventoryComponent->GetInventory(OutInventory);
}

int32 APlayerCharacter::GetInventorySlotsRemaining()
{
	return InventoryComponent->GetNumRemaining();
}

bool APlayerCharacter::AddItem(UItemData *Item)
{
	return InventoryComponent->AddItem(Item);
}

bool APlayerCharacter::RemoveItem(UItemData *Item)
{
	return InventoryComponent->RemoveItem(Item);
}

void APlayerCharacter::SwapInventoryItems(int32 FirstIndex, int32 SecondIndex)
{
	InventoryComponent->SwapItems(FirstIndex, SecondIndex);
}

float APlayerCharacter::BlendViewToSceneCaptureComponent(USceneCaptureComponent2D *SceneCaptureComponent)
{
	CHECK_NULLPTR_RETVAL(ViewBlenderComponent, LogPlayerCharacter, "PlayerCharacter:: No ViewBlenderComponent!", 0.0f);
	return ViewBlenderComponent->BlendToNewView(SceneCaptureComponent);
}

float APlayerCharacter::ResetCameraLocation()
{
	CHECK_NULLPTR_RETVAL(ViewBlenderComponent, LogPlayerCharacter, "PlayerCharacter:: No ViewBlenderComponent!", 0.0f);
	return ViewBlenderComponent->BlendToDefaultView();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "ViewBlenderComponent.h"
#include "FlagManagerComponent.h"
#include "SpawnerComponent.h"
#include "ProjectCalm/ProjectCalmGameInstance.h"
#include "ProjectCalm/Inventory/InventoryComponent.h"
#include "ProjectCalm/Inventory/EquipmentInterface.h"
#include "ProjectCalm/Inventory/PhotoCameraEquipment.h"
#include "ProjectCalm/Inventory/CameraFlash.h"
#include "ProjectCalm/Inventory/CameraLens.h"
#include "ProjectCalm/Inventory/MeshSockets.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

DEFINE_LOG_CATEGORY(LogPlayerCharacter)

#define ABORT_IF_MENU_OPEN() {\
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);\
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "PlayerCharacter:: No Game Instance found!");\
	if (GameInstance->IsPopupMenuOpen()) {return;}}


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	//Setup first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CHECK_NULLPTR_RET(FirstPersonCamera, LogPlayerCharacter, "PlayerCharacter:: Failed to create camera component!");
	
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-25.0f, 0.0f, 65.0f));
	FirstPersonCamera->bUsePawnControlRotation = true;
	
	// Setup player mesh
	USkeletalMeshComponent* CharacterMesh = GetMesh();
	if (CharacterMesh != nullptr)
	{
		CharacterMesh->SetupAttachment(FirstPersonCamera);
		CharacterMesh->SetOnlyOwnerSee(true);
		CharacterMesh->bCastDynamicShadow = false;
		CharacterMesh->CastShadow = false;
		CharacterMesh->SetRelativeRotation(FRotator(0.0f, 270.0f, 0.0f));
		CharacterMesh->SetRelativeLocation(FVector(15.0f, 0.0f, -160.0f));
	}
	
	// Setup Actor Components
	ViewBlenderComponent = CreateDefaultSubobject<UViewBlenderComponent>(TEXT("ViewBlenderComponent"));
	if (ViewBlenderComponent != nullptr) {ViewBlenderComponent->SetCharacterEyes(FirstPersonCamera);}	
	
	FlagManagerComponent = CreateDefaultSubobject<UFlagManagerComponent>(TEXT("FlagManagerComponent"));

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("InventoryComponent"));

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
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);	
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Pause);
		EnhancedInputComponent->BindAction(OpenInventoryAction, ETriggerEvent::Triggered, this, &APlayerCharacter::OpenInventory);
	}
}

void APlayerCharacter::Jump()
{
	ABORT_IF_MENU_OPEN();
	Super::Jump();
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

void APlayerCharacter::Move(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	ABORT_IF_MENU_OPEN();
	CHECK_NULLPTR_RET(Controller, LogPlayerCharacter, "PlayerCharacter:: No Controller!");

	FVector2D Direction = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Direction.Y);
	AddMovementInput(GetActorRightVector(), Direction.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	ABORT_IF_MENU_OPEN();
	CHECK_NULLPTR_RET(Controller, LogPlayerCharacter, "PlayerCharacter:: No Controller!");

	FVector2D Direction = Value.Get<FVector2D>();
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);
}

void APlayerCharacter::Pause(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "Could not get ProjectCalmGameInstance!");

	if (GameInstance->IsPopupMenuOpen()) {GameInstance->ClosePopupMenu();}
	else {GameInstance->LoadPauseMenu();}
}

void APlayerCharacter::OpenInventory(const FInputActionValue &Value)
{
	UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
	CHECK_NULLPTR_RET(GameInstance, LogPlayerCharacter, "Could not get ProjectCalmGameInstance!");

	if (!GameInstance->IsPopupMenuOpen()) {GameInstance->LoadInventoryMenu();}
}

bool APlayerCharacter::AttachEquipment(IEquipmentInterface *Equipment, FName SocketName)
{
	if (SocketName != SOCKET_PLAYER_GRIP) {return false;}
	if (AActor* EquipmentActor = Cast<AActor>(Equipment))
	{
		EquipmentActor->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SocketName);
		EquippedItem = Equipment;
		return true;
	}
	return false;
}

void APlayerCharacter::GetInventory(TArray<UItemData*>& OutInventory)
{
	CHECK_NULLPTR_RET(InventoryComponent, LogPlayerCharacter, "PlayerCharacter:: No InventoryComponent!");
	InventoryComponent->GetInventory(OutInventory);
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

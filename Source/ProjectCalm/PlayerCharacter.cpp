// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "EquipmentInterface.h"
#include "ViewBlenderComponent.h"
#include "FlagManagerComponent.h"
#include "PhotoCameraEquipment.h"
#include "CameraFlash.h"
#include "CameraLens.h"

#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.f);

	//Setup first person camera
	FirstPersonCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	if (FirstPersonCamera == nullptr) {return;}
	
	FirstPersonCamera->SetupAttachment(GetCapsuleComponent());
	FirstPersonCamera->SetRelativeLocation(FVector(-20.0f, 0.0f, 65.0f));
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

	// Temporarily add camera with lens and flash to player on startup
	// TODO: Add Menu System for adding equipment
    ConstructorHelpers::FClassFinder<APhotoCameraEquipment> CameraBPClass(TEXT("/Game/ProjectCalm/Blueprints/BP_PhotoCameraEquipment_Basic"));
    if (!ensure(CameraBPClass.Class != nullptr)) {return;}
    PhotoCameraClass = CameraBPClass.Class;
	
    ConstructorHelpers::FClassFinder<ACameraLens> CameraLensBPClass(TEXT("/Game/ProjectCalm/Blueprints/BP_CameraLensBasic"));
    if (!ensure(CameraLensBPClass.Class != nullptr)) {return;}
    CameraLensClass = CameraLensBPClass.Class;
	
    ConstructorHelpers::FClassFinder<ACameraFlash> CameraFlashBPClass(TEXT("/Game/ProjectCalm/Blueprints/BP_CameraFlash"));
    if (!ensure(CameraFlashBPClass.Class != nullptr)) {return;}
    CameraFlashClass = CameraFlashBPClass.Class;
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
		if (Subsystem != nullptr)
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	// Temporarily add camera with lens and flash to player on startup
	// TODO: Add Menu System for adding equipment
	APhotoCameraEquipment* PhotoCamera = GetWorld()->SpawnActor<APhotoCameraEquipment>(PhotoCameraClass);
	if (PhotoCamera != nullptr)
	{
		EquippedItem = Cast<IEquipmentInterface>(PhotoCamera);
		EquippedItem->Equip(this, TEXT("GripPoint"));
	
		ACameraFlash* CameraFlash = GetWorld()->SpawnActor<ACameraFlash>(CameraFlashClass);
		if (CameraFlash != nullptr)
		{
			IEquipmentInterface* FlashInterface = Cast<IEquipmentInterface>(CameraFlash);
			FlashInterface->Equip(PhotoCamera, TEXT("FlashPoint"));
			PhotoCamera->SetAttachedCameraFlash(CameraFlash);
		}
	
		ACameraLens* CameraLens = GetWorld()->SpawnActor<ACameraLens>(CameraLensClass);
		if (CameraLens != nullptr)
		{
			IEquipmentInterface* LensInterface = Cast<IEquipmentInterface>(CameraLens);
			LensInterface->Equip(PhotoCamera, TEXT("LensPoint"));
			PhotoCamera->SetAttachedCameraLens(CameraLens);
		}
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

		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	}

}

bool APlayerCharacter::GetFlag(FName FlagName)
{
	if (FlagManagerComponent == nullptr) {return false;}
	return FlagManagerComponent->GetFlag(FlagName);
}

void APlayerCharacter::SetFlag(FName FlagName, bool Value)
{	
	if (FlagManagerComponent == nullptr) {return;}
	FlagManagerComponent->SetFlag(FlagName, Value);
	
	UE_LOG(LogTemp, Display, TEXT("All Flags: %s"), *(FlagManagerComponent->GetAllFlagsString()));
}

void APlayerCharacter::Move(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	if (Controller == nullptr) {return;}

	FVector2D Direction = Value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), Direction.Y);
	AddMovementInput(GetActorRightVector(), Direction.X);
}

void APlayerCharacter::Look(const FInputActionValue& Value)  // Automatically applied. Do not call in Tick.
{
	if (Controller == nullptr) {return;}

	FVector2D Direction = Value.Get<FVector2D>();
	AddControllerYawInput(Direction.X);
	AddControllerPitchInput(Direction.Y);
}

float APlayerCharacter::BlendViewToSceneCaptureComponent(USceneCaptureComponent2D *SceneCaptureComponent)
{
	if (ViewBlenderComponent == nullptr) {return 0.0f;}
	return ViewBlenderComponent->BlendToNewView(SceneCaptureComponent);
}

float APlayerCharacter::ResetCameraLocation()
{
	if (ViewBlenderComponent == nullptr) {return 0.0f;}
	return ViewBlenderComponent->BlendToDefaultView();
}
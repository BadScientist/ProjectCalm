// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "PlayerCharacter.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


// Sets default values
AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	RootComponent = EquipmentMesh;
}

void AEquipment::Equip(AActor* OwningActor, FName SocketName)
{
	if (OwningActor == nullptr) {return;}

	SetOwner(OwningActor);

	USceneComponent* ParentComp = nullptr;
	if (ACharacter* Character = Cast<ACharacter>(OwningActor))
	{
		ParentComp = Cast<USceneComponent>(Character->GetMesh());
	}
	else if (AEquipment* Equipment = Cast<AEquipment>(OwningActor))
	{
		ParentComp = Cast<USceneComponent>(Equipment->GetEquipmentMesh());
	}

	if (ParentComp != nullptr)
	{
		AttachToComponent(ParentComp, FAttachmentTransformRules::KeepRelativeTransform, SocketName);
	}

	SetupPlayerControls();
}

APlayerCharacter* AEquipment::GetPlayerCharacter()
{
	if (AActor* OwningActor = GetOwner())
	{
		if (APlayerCharacter* OwningCharacter = Cast<APlayerCharacter>(OwningActor))
		{
			return OwningCharacter;
		}
		else if (AEquipment* OwningEquipment = Cast<AEquipment>(OwningActor))
		{
			return OwningEquipment->GetPlayerCharacter();
		}
	}

	UE_LOG(LogTemp, Error, TEXT("%s: NO OWNING CHARACTER"), *GetActorNameOrLabel());
	return nullptr;
}

APlayerController* AEquipment::GetPlayerController()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	
	if (PlayerCharacter == nullptr) {return nullptr;}
	return Cast<APlayerController>(PlayerCharacter->GetController());
}

UCameraComponent* AEquipment::GetPlayerCamera()
{
	APlayerCharacter* PlayerCharacter = GetPlayerCharacter();
	
	if (PlayerCharacter == nullptr) {return nullptr;}
	return PlayerCharacter->GetCameraComponent();
}

UEnhancedInputComponent* AEquipment::GetEnhancedInputComponent()
{
	APlayerController* PlayerController = GetPlayerController();

	if (PlayerController == nullptr) {return nullptr;}
	return Cast<UEnhancedInputComponent>(PlayerController->InputComponent);
}

UEnhancedInputLocalPlayerSubsystem* AEquipment::GetEnhancedInputLocalPlayerSubsystem()
{
	APlayerController* PlayerController = GetPlayerController();

	if (PlayerController == nullptr) {return nullptr;}
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
}

void AEquipment::SetupPlayerControls()
{
	if (EquipmentMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = GetEnhancedInputLocalPlayerSubsystem())
		{
			Subsystem->AddMappingContext(EquipmentMappingContext, 1);
		}
	}

	if (EquipmentPrimaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
		{		
			EnhancedInputComponent->BindAction(EquipmentPrimaryAction, ETriggerEvent::Started, this, &AEquipment::PrimaryAction);
			EnhancedInputComponent->BindAction(EquipmentPrimaryAction, ETriggerEvent::Completed, this, &AEquipment::PrimaryAction);
		}
	}

	if (EquipmentSecondaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = GetEnhancedInputComponent())
		{
			EnhancedInputComponent->BindAction(EquipmentSecondaryAction, ETriggerEvent::Started, this, &AEquipment::SecondaryAction);
			EnhancedInputComponent->BindAction(EquipmentSecondaryAction, ETriggerEvent::Completed, this, &AEquipment::SecondaryAction);
		}
	}
}

bool AEquipment::GetPlayerFlag(FName FlagName)
{
	if (APlayerCharacter* OwningCharacter = GetPlayerCharacter()) {return OwningCharacter->GetFlag(FlagName);}
    return false;
}

void AEquipment::SetPlayerFlag(FName FlagName, bool Value)
{
	if (APlayerCharacter* OwningCharacter = GetPlayerCharacter()) 
	{
		OwningCharacter->SetFlag(FlagName, Value);
	}
}

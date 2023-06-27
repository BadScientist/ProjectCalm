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

bool AEquipment::GetFlagByName(FName SearchName)
{
	if (EquipmentFlags.IsEmpty()) {return false;}

	for (FEquipmentFlag Flag : EquipmentFlags)
	{
		if (Flag.FlagName == SearchName) {return Flag.Value;}
	}

	return false;
}

void AEquipment::SetFlag(FName SearchName, bool NewValue)
{
	if (EquipmentFlags.IsEmpty()) {return;}

	for (int32 i = 0; i < EquipmentFlags.Num(); i++)
	{		
		if (EquipmentFlags[i].FlagName == SearchName) 
		{
			EquipmentFlags[i].Value = NewValue;
			return;
		}
	}
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
	return Cast<APlayerCharacter>(GetOwner());
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

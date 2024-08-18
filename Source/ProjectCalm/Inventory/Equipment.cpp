// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "EquipReply.h"
#include "EquipperInterface.h"
#include "MeshSockets.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"


// Sets default values
AEquipment::AEquipment()
{
	PrimaryActorTick.bCanEverTick = false;

	EquipmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EquipmentMesh"));
	EquipmentSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("EquipmentSkeletalMesh"));
	if (EquipmentMesh != nullptr)
	{
		SetRootComponent(EquipmentMesh);
		if (EquipmentSkeletalMesh != nullptr) {EquipmentSkeletalMesh->SetupAttachment(EquipmentMesh);}
	}
}

EEquipReply AEquipment::Equip_Internal(AActor* OwningActor)
{
	EEquipReply Response = EEquipReply::FAILED_DEFAULT;
	CHECK_NULLPTR_RETVAL(OwningActor, LogEquipment, "Equipment:: No OwningActor!", Response);

	SetOwner(OwningActor);

	if (IEquipperInterface* Equipper = Cast<IEquipperInterface>(OwningActor))
	{
		if (Equipper->AttachEquipment(this, SOCKET_PLAYER_GRIP))
		{
			Response = EEquipReply::SUCCESS;
			SetupPlayerControls();
		}
	}

	return Response;
}

EEquipReply AEquipment::Equip(APlayerCharacter *OwningCharacter)
{
	if (IEquipmentInterface* EquippedItem = OwningCharacter->GetEquippedItem()) {EquippedItem->Unequip();}
	OwningPlayerCharacter = OwningCharacter;
	return Equip_Internal(OwningCharacter);
}

void AEquipment::Unequip()
{
	if (IEquipperInterface* Equipper = Cast<IEquipperInterface>(GetOwner()))
	{
		Equipper->RemoveEquipment(this);
	}

	ResetPlayerControls();
	Destroy();
}

void AEquipment::SetupPlayerControls()
{
	if (EquipmentMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PCPlayerStatics::GetEnhancedInputLocalPlayerSubsystem(this))
		{
			Subsystem->AddMappingContext(EquipmentMappingContext, 1);
		}
	}

	if (EquipmentPrimaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
		{		
			PrimaryInputStartBinding = &(EnhancedInputComponent->BindAction(EquipmentPrimaryAction, ETriggerEvent::Started, this, &AEquipment::PrimaryAction));
			PrimaryInputCompletedBinding = &(EnhancedInputComponent->BindAction(EquipmentPrimaryAction, ETriggerEvent::Completed, this, &AEquipment::PrimaryAction));
		}
	}

	if (EquipmentSecondaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
		{
			SecondaryInputStartBinding = &(EnhancedInputComponent->BindAction(EquipmentSecondaryAction, ETriggerEvent::Started, this, &AEquipment::SecondaryAction));
			SecondaryInputCompletedBinding = &(EnhancedInputComponent->BindAction(EquipmentSecondaryAction, ETriggerEvent::Completed, this, &AEquipment::SecondaryAction));
		}
	}
}

void AEquipment::ResetPlayerControls()
{
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PCPlayerStatics::GetEnhancedInputLocalPlayerSubsystem(this))
	{
		if (EquipmentMappingContext != nullptr) {Subsystem->RemoveMappingContext(EquipmentMappingContext);}
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
	{
		if (PrimaryInputStartBinding != nullptr)
		{
			EnhancedInputComponent->RemoveActionBindingForHandle(PrimaryInputStartBinding->GetHandle());
			PrimaryInputStartBinding = nullptr;
		}
		if (PrimaryInputCompletedBinding != nullptr)
		{
			EnhancedInputComponent->RemoveActionBindingForHandle(PrimaryInputCompletedBinding->GetHandle());
			PrimaryInputCompletedBinding = nullptr;
		}
		if (SecondaryInputStartBinding != nullptr)
		{
			EnhancedInputComponent->RemoveActionBindingForHandle(SecondaryInputStartBinding->GetHandle());
			SecondaryInputStartBinding = nullptr;
		}
		if (SecondaryInputCompletedBinding != nullptr)
		{
			EnhancedInputComponent->RemoveActionBindingForHandle(SecondaryInputCompletedBinding->GetHandle());
			SecondaryInputCompletedBinding = nullptr;
		}
	}
}

bool AEquipment::GetPlayerFlag(FName FlagName)
{
	if (OwningPlayerCharacter != nullptr) {return OwningPlayerCharacter->GetFlag(FlagName);}
    return false;
}

void AEquipment::SetPlayerFlag(FName FlagName, bool Value)
{
	if (OwningPlayerCharacter != nullptr) {OwningPlayerCharacter->SetFlag(FlagName, Value);}
}

UMeshComponent* AEquipment::GetEquipmentMesh()
{
    if (EquipmentSkeletalMesh != nullptr && EquipmentSkeletalMesh->GetSkeletalMeshAsset() != nullptr) {return EquipmentSkeletalMesh;}
	return EquipmentMesh;
}

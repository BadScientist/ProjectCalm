// Fill out your copyright notice in the Description page of Project Settings.


#include "Equipment.h"
#include "EquipReply.h"
#include "EquipperInterface.h"
#include "MeshSockets.h"
#include "ProjectCalm/Utilities/LogMacros.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"
#include "ProjectCalm/Utilities/PCPlayerStatics.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Kismet/GameplayStatics.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif


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

#ifdef LOCAL_DEBUG_LOGS
	UE_LOG(LogTemp, Display, TEXT("%s: Attempting to equip to %s..."), *GetActorNameOrLabel(), *(OwningActor->GetActorNameOrLabel()));
#endif // DEBUG

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
#ifdef LOCAL_DEBUG_LOGS
			UE_LOG(LogEquipment, Display, TEXT("%s: Setting up player controls..."), *GetActorNameOrLabel());
#endif // DEBUG

	if (EquipmentMappingContext != nullptr)
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = PCPlayerStatics::GetEnhancedInputLocalPlayerSubsystem(this))
		{
#ifdef LOCAL_DEBUG_LOGS
			UE_LOG(LogEquipment, Display, TEXT("%s: Adding Equipment IMC..."), *GetActorNameOrLabel());
#endif // DEBUG
			Subsystem->AddMappingContext(EquipmentMappingContext, 1);
		}
	}

	if (EquipmentPrimaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
		{
#ifdef LOCAL_DEBUG_LOGS
			UE_LOG(LogEquipment, Display, TEXT("%s: Binding Primary Input Action..."), *GetActorNameOrLabel());
#endif // DEBUG
			PrimaryInputStartBinding = &(EnhancedInputComponent->BindAction(EquipmentPrimaryAction, ETriggerEvent::Triggered, this, &AEquipment::PrimaryAction));
		}
	}

	if (EquipmentSecondaryAction != nullptr)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = PCPlayerStatics::GetEnhancedInputComponent(this))
		{
#ifdef LOCAL_DEBUG_LOGS
			UE_LOG(LogEquipment, Display, TEXT("%s: Binding Secondary Input Action..."), *GetActorNameOrLabel());
#endif // DEBUG
			SecondaryInputStartBinding = &(EnhancedInputComponent->BindAction(EquipmentSecondaryAction, ETriggerEvent::Triggered, this, &AEquipment::SecondaryAction));
		}
	}
}

void AEquipment::ResetPlayerControls()
{
#ifdef LOCAL_DEBUG_LOGS
			UE_LOG(LogEquipment, Display, TEXT("%s: Resetting Player Controls..."), *GetActorNameOrLabel());
#endif // DEBUG
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

void AEquipment::PlayPrimaryActionSound()
{
    if (!PrimaryActionSound.IsNone())
    {
        UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
        CHECK_NULLPTR_RET(GameInstance, LogEquipment, "Equipment:: No Game Instance found!");
        GameInstance->PlayDiageticSound(PrimaryActionSound, this, GetActorLocation());
    }
}

void AEquipment::PlaySecondaryActionSound()
{
    if (!SecondaryActionSound.IsNone())
    {
        UProjectCalmGameInstance* GameInstance = PCGameStatics::GetPCGameInstance(this);
        CHECK_NULLPTR_RET(GameInstance, LogEquipment, "Equipment:: No Game Instance found!");
        GameInstance->PlayDiageticSound(SecondaryActionSound, this, GetActorLocation());
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

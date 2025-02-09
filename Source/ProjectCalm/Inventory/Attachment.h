// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Equipment.h"

#include "Attachment.generated.h"

enum EEquipReply : uint8;


UCLASS()
class PROJECTCALM_API AAttachment : public AEquipment
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAttachment(){};

protected:
	FName TargetSocket {FName()};

	// START EQUIPMENT INTERFACE
	virtual EEquipReply Equip(APlayerCharacter* OwningCharacter) override;
	// END EQUIPMENT INTERFACE

	virtual EEquipReply Equip_Internal(AActor* OwningActor) override;

};

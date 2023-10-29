// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FlagManagerComponent.generated.h"


USTRUCT()
struct FInfoFlag
{
	GENERATED_BODY();

	FInfoFlag(){};
	FInfoFlag(FName NewFlagName, bool InitialValue)
	{
		FlagName = NewFlagName;
		Value = InitialValue;
	};

	UPROPERTY(EditAnywhere)
	FName FlagName;
	UPROPERTY(EditAnywhere)
	bool Value {false};
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API UFlagManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UFlagManagerComponent();

protected:
	UPROPERTY(EditAnywhere, Category = Flags)
	TArray<FInfoFlag> InfoFlags;

public:
	bool GetFlag(FName SearchName);
	void SetFlag(FName SearchName, bool Value);

	FString GetAllFlagsString();
		
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagManagerComponent.h"

// Sets default values for this component's properties
UFlagManagerComponent::UFlagManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

bool UFlagManagerComponent::GetFlagByName(FName SearchName)
{
	if (InfoFlags.IsEmpty()) {return false;}

	for (FInfoFlag Flag : InfoFlags)
	{
		if (Flag.FlagName == SearchName) {return Flag.Value;}
	}

	return false;
}

void UFlagManagerComponent::SetFlag(FName SearchName, bool NewValue)
{
	for (int32 i = 0; i < InfoFlags.Num(); i++)
	{		
		if (InfoFlags[i].FlagName == SearchName) 
		{
			InfoFlags[i].Value = NewValue;
			return;
		}
	}

	InfoFlags.Add(FInfoFlag(SearchName, NewValue));
}

FString UFlagManagerComponent::GetAllFlagsString()
{
	FString ResultString = "";

	for (FInfoFlag Flag : InfoFlags)
	{
		FString FlagString = FString::Printf(TEXT("%s: %s | "), *Flag.FlagName.ToString(), Flag.Value ? TEXT("true") : TEXT("false"));
		ResultString.Append(FlagString);
	}

    return ResultString;
}

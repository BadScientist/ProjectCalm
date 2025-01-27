// Fill out your copyright notice in the Description page of Project Settings.


#include "FlagManagerComponent.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#ifdef PC_DEBUG_LOGS
	// #define LOCAL_DEBUG_LOGS
#endif

// Sets default values for this component's properties
UFlagManagerComponent::UFlagManagerComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UFlagManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
#ifdef LOCAL_DEBUG_LOGS
	FString CurrentLog = FString::Printf(TEXT("FlagManagerComponent::%s"), *GetAllFlagsString());

	if (CurrentLog != LastLog)
	{
		UE_LOG(LogTemp, Display, TEXT("%s"), *CurrentLog);
		LastLog = CurrentLog;
	}
#endif
}

bool UFlagManagerComponent::GetFlag(FName SearchName)
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
		FString FlagString = FString::Printf(TEXT("%s=%i|"), *Flag.FlagName.ToString(), Flag.Value);
		ResultString.Append(FlagString);
	}

    return ResultString;
}

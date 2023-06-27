// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCalmGameMode.h"
#include "ProjectCalmCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectCalmGameMode::AProjectCalmGameMode()
	: Super()
{
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (Start)"));
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (End)"));
}

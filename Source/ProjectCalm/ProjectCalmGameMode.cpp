// Copyright Epic Games, Inc. All Rights Reserved.

#include "ProjectCalmGameMode.h"
#include "ProjectCalmCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProjectCalmGameMode::AProjectCalmGameMode()
	: Super()
{
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (Start)"));
	
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/ProjectCalm/Blueprints/Player/BP_PlayerCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	
	UE_LOG(LogTemp, Display, TEXT("Building PCGM (End)"));
}

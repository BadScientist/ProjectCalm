// Copyright 2025 Joseph D Tong aka "BadScientist"

#include "PausableCharacter.h"
#include "ProjectCalm/Utilities/PCGameStatics.h"


APausableCharacter::APausableCharacter(const FObjectInitializer &ObjectInitializer) : Super(ObjectInitializer)
{
}

void APausableCharacter::BeginPlay()
{
	Super::BeginPlay();
    
	if (AProjectCalmGameMode* GameMode = PCGameStatics::GetPCGameMode(this))
	{
		GameMode->OnGamePaused.AddDynamic(this, &APausableCharacter::OnGamePaused);
		GameMode->OnGameUnpaused.AddDynamic(this, &APausableCharacter::OnGameUnpaused);
	}
}

void APausableCharacter::OnGamePaused(float InTimeDilation)
{
	CustomTimeDilation = InTimeDilation;
}

void APausableCharacter::OnGameUnpaused(float InTimeDilation)
{
	CustomTimeDilation = InTimeDilation;
}

#include "SpawnRegionVisualizerComponent.h"

USpawnRegionVisualizerComponent::USpawnRegionVisualizerComponent()
{
}

void USpawnRegionVisualizerComponent::UpdateProperties(FVector InLocation, FRotator InRotation, FVector InSize)
{
    RegionLocation = InLocation;
    RegionRotation = InRotation;
    RegionSize = InSize;
}
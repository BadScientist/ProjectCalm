// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerComponent.h"
#include "ProjectCalm/Characters/PhotoSubjects/PhotoSubjectSpawnRegion.h"
#include "ProjectCalm/Utilities/LogMacros.h"

#include "Kismet/GameplayStatics.h"


// Sets default values for this component's properties
USpawnerComponent::USpawnerComponent()
{
	// Set this component to be initialized when the game starts
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USpawnerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	CleanupSpawns();
	
	TickCount++;
}

// Called when the game starts
void USpawnerComponent::BeginPlay()
{
	Super::BeginPlay();

	TSubclassOf<APhotoSubjectSpawnRegion> SpawnRegionClass = APhotoSubjectSpawnRegion::StaticClass();
	TArray<AActor*> OutArray;
	UGameplayStatics::GetAllActorsOfClass(this, SpawnRegionClass, OutArray);

	for (AActor* Actor : OutArray)
	{
		APhotoSubjectSpawnRegion* SpawnRegion = Cast<APhotoSubjectSpawnRegion>(Actor);
		
		if (SpawnRegion == nullptr) {continue;}
		SpawnRegion->Initialize();
		SpawnRegions.Add(SpawnRegion);
	}

	StartSpawnTimer();	
}

void USpawnerComponent::StartSpawnTimer()
{
    UWorld* World = GetWorld();
    CHECK_NULLPTR_RET(World, LogActorComponent, "SpawnerComponent:: Could not get World!");

    World->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&USpawnerComponent::SpawnPhotoSubject,
		SpawnCooldown + FMath::RandRange(-1 * SpawnCooldown * SpawnCooldownRangeFactor, SpawnCooldown * SpawnCooldownRangeFactor));
}

void USpawnerComponent::SpawnPhotoSubject()
{
	if (SpawnRegions.IsEmpty() || CurrentSpawns >= MaxSpawns) {return;}
	
	FVector SpawnLocation = GetComponentLocation();
	FVector Offset = GetForwardVector().RotateAngleAxis(FMath::RandRange(MinDirectionOffset, 360 - MinDirectionOffset), GetUpVector());
	Offset *= FMath::RandRange(SpawnLocationMinOffset, SpawnLocationMaxOffset);
	SpawnLocation += Offset;

	TArray<APhotoSubjectSpawnRegion*> ValidRegions;
	for (APhotoSubjectSpawnRegion* SpawnRegion : SpawnRegions)
	{
		if (SpawnRegion->Contains2D(SpawnLocation))
		{
			ValidRegions.Add(SpawnRegion);
		}
	}

	if (!ValidRegions.IsEmpty())
	{
		int32 RegionIdx = FMath::RandRange(0, ValidRegions.Num() - 1);	
		if (ValidRegions[RegionIdx]->SpawnPhotoSubject(SpawnLocation)) {CurrentSpawns++;}
	}

	StartSpawnTimer();
}

void USpawnerComponent::CleanupSpawns()
{
	if (SpawnRegions.IsEmpty()) {return;}

	APhotoSubjectSpawnRegion* SpawnRegion = SpawnRegions[TickCount % SpawnRegions.Num()];
	int32 DespawnedActorCount = SpawnRegion->CleanupSpawns(GetOwner());
	CurrentSpawns -= DespawnedActorCount;
}

TArray<AActor*> USpawnerComponent::GetAllPhotoSubjects()
{
	TArray<AActor*> Spawns;
	for (APhotoSubjectSpawnRegion* SpawnRegion : SpawnRegions)
	{
		if (SpawnRegion == nullptr) {continue;}
		Spawns.Append(SpawnRegion->GetSpawnedSubjects());
	}

    return Spawns;
}

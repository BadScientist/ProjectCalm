// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnerComponent.h"
#include "PhotoSubjectSpawnRegion.h"

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
    GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&USpawnerComponent::SpawnPhotoSubject,
		SpawnCooldown + FMath::RandRange(-1 * SpawnCooldown * SpawnCooldownRangeFactor, SpawnCooldown * SpawnCooldownRangeFactor));
}

void USpawnerComponent::SpawnPhotoSubject()
{
	if (SpawnRegions.IsEmpty()) {return;}
	
	FVector SpawnLocation = GetComponentLocation();
	FVector Offset = GetForwardVector().RotateAngleAxis(FMath::RandRange(50, 310), GetUpVector());
	Offset *= FMath::RandRange(SpawnLocationMinOffset, SpawnLocationMaxOffset);
	SpawnLocation += Offset;

	UE_LOG(LogTemp, Warning, TEXT("SpawnerComponent:: %f - Requesting Spawn At: %s"), GetWorld()->GetTimeSeconds(), *SpawnLocation.ToCompactString());
    DrawDebugPoint(GetWorld(), SpawnLocation, 30, FColor::Red, false, 5.0f);
	for (APhotoSubjectSpawnRegion* SpawnRegion : SpawnRegions)
	{
		if (CurrentSpawns < MaxSpawns && SpawnRegion->Contains2D(SpawnLocation))
		{
			if (SpawnRegion->SpawnPhotoSubject(SpawnLocation)) 
			{
				UE_LOG(LogTemp, Display, TEXT("SpawnerComponent:: New Spawn"));
				CurrentSpawns++;
			}
			
			StartSpawnTimer();
			return;
		}
	}

	StartSpawnTimer();
}

void USpawnerComponent::CleanupSpawns()
{
	if (SpawnRegions.IsEmpty()) {return;}
	UE_LOG(LogTemp, Error, TEXT("SpawnerComponent:: Cleaning up spawns in region %i"), TickCount % SpawnRegions.Num());

	APhotoSubjectSpawnRegion* SpawnRegion = SpawnRegions[TickCount % SpawnRegions.Num()];
	int32 DespawnedActorCount = SpawnRegion->CleanupSpawns(GetComponentLocation(), GetForwardVector());
	CurrentSpawns -= DespawnedActorCount;
}

TArray<AActor*> USpawnerComponent::GetAllPhotoSubjects()
{
	if (SpawnRegions.IsEmpty()) {UE_LOG(LogTemp, Error, TEXT("SpawnerComponent:: No SpawnRegions!"));}
	
	TArray<AActor*> Spawns;
	for (APhotoSubjectSpawnRegion* SpawnRegion : SpawnRegions)
	{
		if (SpawnRegion == nullptr) {continue;}
		Spawns.Append(SpawnRegion->GetSpawnedSubjects());
	}

	if (Spawns.IsEmpty()) {UE_LOG(LogTemp, Error, TEXT("SpawnerComponent:: No Spawns!"));}

    return Spawns;
}

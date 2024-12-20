// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "SpawnerComponent.generated.h"

class APhotoSubjectSpawnRegion;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API USpawnerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnerComponent();
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawns{15};

	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	float MinDirectionOffset{50.0f};

	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	float SpawnLocationMinOffset{3000.0f};

	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	float SpawnLocationMaxOffset{6000.0f};

	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = "true"))
	float SpawnCooldown{1.0f};

	UPROPERTY(EditAnywhere, Category = Spawning, meta = (AllowPrivateAccess = "true"), meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
	float SpawnCooldownRangeFactor{0.5f};

	int32 CurrentSpawns{0};
	int32 TickCount{0};
	FTimerHandle SpawnTimerHandle;

	TArray<APhotoSubjectSpawnRegion*> SpawnRegions;

	void StartSpawnTimer();
	void SpawnPhotoSubject();
	void CleanupSpawns();

public:
	TArray<AActor*> GetAllPhotoSubjects();

		
};

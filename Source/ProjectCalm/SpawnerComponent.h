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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = SpawnRegions, meta = (AllowPrivateAccess = "true"))
	int32 MaxSpawns{15};

	int32 CurrentSpawns{0};
	int32 TickCount{0};
	float MinDirectionOffset{45.0f};
	float SpawnLocationMinOffset{1000.0f};
	float SpawnLocationMaxOffset{1500.0f};
	float SpawnCooldown{5.0f};
	float SpawnCooldownRangeFactor{0.5f};
	FTimerHandle SpawnTimerHandle;

	TArray<APhotoSubjectSpawnRegion*> SpawnRegions;

	void StartSpawnTimer();
	void SpawnPhotoSubject();
	void CleanupSpawns();

public:
	TArray<AActor*> GetAllPhotoSubjects();

		
};

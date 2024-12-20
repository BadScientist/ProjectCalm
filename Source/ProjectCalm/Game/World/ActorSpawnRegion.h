// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#if WITH_EDITORONLY_DATA
#include "ProjectCalm/EditorOnly/SpawnRegionVisualizerComponent.h"
#endif

#include "SpawnRegionInterface.h"
#include "ActorSpawnRegion.generated.h"


USTRUCT()
struct FSpawnInfo
{
    GENERATED_BODY()

    FSpawnInfo(){};

    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> ActorClass;

    UPROPERTY(EditAnywhere)
    TArray<TEnumAsByte<EPhysicalSurface>> ValidSurfaces;

    UPROPERTY(EditAnywhere, meta=(ClampMin="1", UIMin="1", ForceUnits="cm"))
    float SweepSphereRadius{300.0f};

    UPROPERTY(EditAnywhere, meta=(ForceUnits="cm"))
    float SurfaceOffset{0.0f};

    UPROPERTY(EditAnywhere, meta=(ClampMin="0", UIMin="0"))
    int32 Count{1};
};


struct FSpawnCell
{
    FSpawnCell(){};
    FSpawnCell(FVector InLocation, FVector InSize)
    {
        Location = InLocation;
        Size = InSize;
    };

    FVector Location{FVector::ZeroVector};
    FVector Size{FVector(1.0f, 1.0f, 1.0f)};

    FVector GetRandomLocation()
    {
        return FVector(FMath::RandRange(Location.X - Size.X / 2, Location.X + Size.X / 2),
            FMath::RandRange(Location.Y - Size.Y / 2, Location.Y + Size.Y / 2),
            Location.Z);
    }
};


UCLASS()
class PROJECTCALM_API AActorSpawnRegion : public AActor, public ISpawnRegionInterface
{
	GENERATED_BODY()

public:
	AActorSpawnRegion();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    TArray<FSpawnInfo> SpawnList;
    UPROPERTY(EditAnywhere)
    FVector Size {FVector(1000.0f, 1000.0f, 1000.0f)};
    UPROPERTY(EditAnywhere)
    int32 VerticalCells{1};
    UPROPERTY(EditAnywhere)
    int32 HorizontalCells{1};
    UPROPERTY(EditAnywhere)
    bool bDestroyAfterSpawning{true};

    TArray<FSpawnCell> CreateSpawnCells();
    bool SpawnActor(FVector Location, FSpawnInfo SpawnInfo);

public:
    // START ISPAWNREGIONINTERFACE IMPLEMENTATION
    FVector GetSize() const override {return Size;};
    FVector GetRegionLocation() const override {return GetActorLocation();};
    FRotator GetRegionRotation() const override {return GetActorRotation();};
	virtual void SetSize(FVector InSize) override {Size = InSize;};
	virtual void SetRegionLocation(FVector InLocation) override {SetActorLocation(InLocation);};
	virtual void SetRegionRotation(FRotator InRotation) override {SetActorRotation(InRotation);};
    // END ISPAWNREGIONINTERFACE IMPLEMENTATION

#if WITH_EDITORONLY_DATA
DEFINE_VISUALIZER
#endif

};
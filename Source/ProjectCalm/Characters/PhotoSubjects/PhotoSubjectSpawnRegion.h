// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "ProjectCalm/Game/World/SpawnRegionInterface.h"
#include "PhotoSubjectSpawnRegion.generated.h"


USTRUCT(BlueprintType)
struct FPhotoSubjectSpawnData
{
	GENERATED_BODY()

    FPhotoSubjectSpawnData(){};
    
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> SubjectClass;
    UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
    int32 Max {0};
    UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1", UIMin = "0.0", UIMax = "1.0"))
    float Weight {0.0f};

    int32 ActiveSpawns {0};
    
};


UCLASS()
class PROJECTCALM_API APhotoSubjectSpawnRegion : public AActor, public ISpawnRegionInterface
{
	GENERATED_BODY()

public:
	APhotoSubjectSpawnRegion();

    void Initialize();

protected:
    virtual void BeginPlay() override;

private:
    UPROPERTY(EditAnywhere)
    TArray<FPhotoSubjectSpawnData> SpawnableSubjects;
    UPROPERTY(EditAnywhere)
    FVector Size {FVector::ZeroVector};

    bool bInitialized = false;
    float TotalWeight {0};
    TArray<AActor*> SpawnedSubjects;

    int32 PickSubject();

public:
    // START ISPAWNREGIONINTERFACE IMPLEMENTATION
    virtual FVector GetSize() const override {return Size;};
    virtual FVector GetRegionLocation() const override {return GetActorLocation();};
    virtual FRotator GetRegionRotation() const override {return GetActorRotation();};
	virtual void SetSize(FVector InSize) override {Size = InSize;};
	virtual void SetRegionLocation(FVector InLocation) override {SetActorLocation(InLocation);};
	virtual void SetRegionRotation(FRotator InRotation) override {SetActorRotation(InRotation);};
    // END ISPAWNREGIONINTERFACE IMPLEMENTATION

    TArray<FPhotoSubjectSpawnData> GetSpawnableSubjects() {return SpawnableSubjects;}
    TArray<AActor*> GetSpawnedSubjects() {return SpawnedSubjects;}

    bool Contains2D(FVector TestLocation);
    bool SpawnPhotoSubject(FVector SpawnLocation);
    int32 CleanupSpawns(AActor* Player);

// private:
//     class USpawnRegionVisualizerComponent* SpawnRegionVisComp{nullptr};

// #if WITH_EDITOR
//     void UpdateVisualizerComponentProperties();

// public:
//     virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
//     virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
//     virtual void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
//     virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent& EditEvent) override;
//     virtual void PostEditUndo() override;
// #endif
};

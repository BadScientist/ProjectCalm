// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "PhotoSubjectSpawnRegion.generated.h"


UENUM(BlueprintType)
enum class SubjectName : uint8
{
    BARREL UMETA(DisplayName = "Barrel"),
    BOX UMETA(DisplayName = "Box")
};

USTRUCT(BlueprintType)
struct FPhotoSubjectSpawnData
{
	GENERATED_BODY()

    FPhotoSubjectSpawnData(){};

    // UPROPERTY(EditAnywhere)
    // SubjectName Name = SubjectName::BARREL;
    UPROPERTY(EditAnywhere)
    TSubclassOf<AActor> SubjectClass;
    UPROPERTY(EditAnywhere, meta=(ClampMin="0"))
    int32 Max {0};
    UPROPERTY(EditAnywhere, meta=(ClampMin="0", ClampMax="1", UIMin = "0.0", UIMax = "1.0"))
    float Weight {0.0f};

    int32 ActiveSpawns {0};

    // void Initialize();
    // FString PathEnumToString();
};


UCLASS()
class PROJECTCALM_API APhotoSubjectSpawnRegion : public AActor
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
    //int32 SpawnLimit {0};

    int32 PickSubject();

public:
    FVector GetSize() {return Size;};
    void SetSize(FVector InSize) {Size = InSize;};
    //int32 GetSpawnLimit() {return SpawnLimit;}
    TArray<FPhotoSubjectSpawnData> GetSpawnableSubjects() {return SpawnableSubjects;}
    TArray<AActor*> GetSpawnedSubjects() {return SpawnedSubjects;}

    bool Contains2D(FVector TestLocation);
    bool SpawnPhotoSubject(FVector SpawnLocation);
    int32 CleanupSpawns(AActor* Player);


#if WITH_EDITORONLY_DATA
// EDITOR-ONLY VISUALIZATION

private:
    class USpawnRegionVisualizerComponent* SpawnRegionVisComp;
    void UpdateVisualizerComponentProperties();

public:
    virtual void EditorApplyTranslation(const FVector & DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
    virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
    virtual void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;
    virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent &EditEvent) override;
    virtual void PostEditUndo() override;
#endif

};
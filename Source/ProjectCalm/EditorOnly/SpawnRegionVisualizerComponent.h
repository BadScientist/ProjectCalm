// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawnRegionVisualizerComponent.generated.h"

#define DEFINE_VISUALIZER \
private:\
    class USpawnRegionVisualizerComponent* SpawnRegionVisComp;\
    void UpdateVisualizerComponentProperties();\
public:\
    virtual void EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;\
    virtual void EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;\
    virtual void EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) override;\
    virtual void PostEditChangeChainProperty(FPropertyChangedChainEvent &EditEvent) override;\
    virtual void PostEditUndo() override;

#define IMPLEMENT_VISUALIZER(ImplementingClass, VectorSize) \
    void ImplementingClass::UpdateVisualizerComponentProperties() {SetActorRotation(FRotator(0, GetActorRotation().Yaw, 0)); if (SpawnRegionVisComp == nullptr) {return;} SpawnRegionVisComp->UpdateProperties(GetActorLocation(), GetActorRotation(), VectorSize);}\
    void ImplementingClass::EditorApplyTranslation(const FVector& DeltaTranslation, bool bAltDown, bool bShiftDown, bool bCtrlDown) {Super::EditorApplyTranslation(DeltaTranslation, bAltDown, bShiftDown, bCtrlDown); UpdateVisualizerComponentProperties();}\
    void ImplementingClass::EditorApplyRotation(const FRotator& DeltaRotation, bool bAltDown, bool bShiftDown, bool bCtrlDown) {Super::EditorApplyRotation(DeltaRotation, bAltDown, bShiftDown, bCtrlDown); UpdateVisualizerComponentProperties();}\
    void ImplementingClass::EditorApplyScale(const FVector& DeltaScale, const FVector* PivotLocation, bool bAltDown, bool bShiftDown, bool bCtrlDown) {const FVector CurrentScale = GetRootComponent()->GetRelativeScale3D(); FVector SafeDeltaScale = FVector(FMath::Clamp(DeltaScale.X, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Y, -1.0f, 1.0f), FMath::Clamp(DeltaScale.Z, -1.0f, 1.0f)); FVector ScalingVector = FVector(1.0f) + SafeDeltaScale; VectorSize *= ScalingVector; if (PivotLocation) {const FRotator ActorRotation = GetActorRotation(); const FVector WorldDelta = GetActorLocation() - (*PivotLocation); const FVector LocalDelta = (ActorRotation.GetInverse()).RotateVector(WorldDelta); const FVector LocalScaledDelta = LocalDelta * (ScalingVector / FVector(1.0f)); const FVector WorldScaledDelta = ActorRotation.RotateVector(LocalScaledDelta); SetActorLocation(WorldScaledDelta + (*PivotLocation));} UpdateVisualizerComponentProperties();}\
    void ImplementingClass::PostEditChangeChainProperty(FPropertyChangedChainEvent &EditEvent) {Super::PostEditChangeChainProperty(EditEvent); if (!EditEvent.PropertyChain.IsEmpty() && EditEvent.PropertyChain.GetHead() != nullptr) {if (FProperty* EditedProperty = EditEvent.PropertyChain.GetHead()->GetValue()) {if (EditedProperty->GetNameCPP() == "RelativeLocation" || EditedProperty->GetNameCPP() == "RelativeRotation" || EditedProperty->GetNameCPP() == "Size") {UpdateVisualizerComponentProperties();}}}}\
    void ImplementingClass::PostEditUndo() {Super::PostEditUndo(); UpdateVisualizerComponentProperties();}


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTCALM_API USpawnRegionVisualizerComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USpawnRegionVisualizerComponent();

private:
    UPROPERTY(VisibleAnywhere)
    FVector RegionLocation = FVector::ZeroVector;
    UPROPERTY(VisibleAnywhere)
    FRotator RegionRotation = FRotator::ZeroRotator;
    UPROPERTY(VisibleAnywhere)
    FVector RegionSize = FVector::ZeroVector;

public:
    void UpdateProperties(FVector InLocation, FRotator InRotation, FVector InSize);
    FVector GetRegionLocation() const {return RegionLocation;}
    FRotator GetRegionRotation() const {return RegionRotation;}
    FVector GetRegionSize() const {return RegionSize;}
    
};

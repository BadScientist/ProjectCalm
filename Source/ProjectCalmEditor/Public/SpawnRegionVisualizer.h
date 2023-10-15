#pragma once

#include "ComponentVisualizer.h"

class APhotoSubjectSpawnRegion;

class FSpawnRegionVisualizer : public FComponentVisualizer
{
public:
    FSpawnRegionVisualizer();
    virtual ~FSpawnRegionVisualizer();

    // virtual void OnRegister() override;
    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click);
    // virtual void EndEditing() override;
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
    // virtual bool GetCustomInputCoordinateSystem(const FEditorViewportClient* ViewportClient, FMatrix& OutMatrix) const override;
    virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;
    // virtual bool HandleInputKey(FEditorViewportClient* ViewportClient, FViewport* Viewport, FKey Key, EInputEvent Event) override;
    // virtual TSharedPtr<SWidget> GenerateContextMenu() const override;
    // virtual void DrawVisualizationHUD(const UActorComponent* Component, const FViewport* Viewport, const FSceneView* View, FCanvas* Canvas);
    // virtual bool IsVisualizingArchetype();

    APhotoSubjectSpawnRegion* GetSpawnRegion() const;

private:
    TSharedPtr<FUICommandList> VisualizerActions;

};
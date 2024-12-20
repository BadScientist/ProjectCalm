#pragma once

#include "ComponentVisualizer.h"

class ISpawnRegionInterface;

class FSpawnRegionVisualizer : public FComponentVisualizer
{
public:
    FSpawnRegionVisualizer();
    virtual ~FSpawnRegionVisualizer();

    virtual void DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI) override;
    virtual bool VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click);
    virtual bool GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const override;
    virtual bool HandleInputDelta(FEditorViewportClient* ViewportClient, FViewport* Viewport, FVector& DeltaTranslate, FRotator& DeltaRotate, FVector& DeltaScale) override;

    ISpawnRegionInterface* GetSpawnRegion() const;

private:
    TSharedPtr<FUICommandList> VisualizerActions;

};
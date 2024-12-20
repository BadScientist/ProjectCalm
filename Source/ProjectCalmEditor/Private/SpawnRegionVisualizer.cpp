#include "SpawnRegionVisualizer.h"
#include "VisualizerProxy.h"

#include "ProjectCalm/Game/World/SpawnRegionInterface.h"
#include "EditorOnly/SpawnRegionVisualizerComponent.h"

FSpawnRegionVisualizer::FSpawnRegionVisualizer()
{
}

FSpawnRegionVisualizer::~FSpawnRegionVisualizer()
{
}

void FSpawnRegionVisualizer::DrawVisualization(const UActorComponent* Component, const FSceneView* View, FPrimitiveDrawInterface* PDI)
{
    if (const USpawnRegionVisualizerComponent* VisualizerComp = Cast<USpawnRegionVisualizerComponent>(Component))
    {
        FVector RegionLocation = VisualizerComp->GetRegionLocation();
        FRotator RegionRotation = VisualizerComp->GetRegionRotation();
        FVector RegionSize = VisualizerComp->GetRegionSize();

        TArray<FVector> Vertices{
            FVector(RegionLocation.X + 0.5 * RegionSize.X, RegionLocation.Y + 0.5 * RegionSize.Y, RegionLocation.Z),
            FVector(RegionLocation.X + 0.5 * RegionSize.X, RegionLocation.Y - 0.5 * RegionSize.Y, RegionLocation.Z),
            FVector(RegionLocation.X - 0.5 * RegionSize.X, RegionLocation.Y - 0.5 * RegionSize.Y, RegionLocation.Z),
            FVector(RegionLocation.X - 0.5 * RegionSize.X, RegionLocation.Y + 0.5 * RegionSize.Y, RegionLocation.Z),
            FVector(RegionLocation.X + 0.5 * RegionSize.X, RegionLocation.Y + 0.5 * RegionSize.Y, RegionLocation.Z - RegionSize.Z),
            FVector(RegionLocation.X + 0.5 * RegionSize.X, RegionLocation.Y - 0.5 * RegionSize.Y, RegionLocation.Z - RegionSize.Z),
            FVector(RegionLocation.X - 0.5 * RegionSize.X, RegionLocation.Y - 0.5 * RegionSize.Y, RegionLocation.Z - RegionSize.Z),
            FVector(RegionLocation.X - 0.5 * RegionSize.X, RegionLocation.Y + 0.5 * RegionSize.Y, RegionLocation.Z - RegionSize.Z)};
        
        for (int32 i = 0; i < Vertices.Num(); i++)
        {
            // Rotate each vertex around RegionLocation using RegionRotation...
            Vertices[i] = RegionLocation - RegionRotation.RotateVector(RegionLocation - Vertices[i]);
        }

        HVisualizerProxy* VisProxy = new HVisualizerProxy(Component);
        PDI->SetHitProxy(VisProxy);

        const FLinearColor Color = VisualizerComp->IsSelectedInEditor() ? FLinearColor::Blue : FLinearColor::Gray;
        for (int32 i = 0; i < 4; i++)
        {
            PDI->DrawLine(Vertices[i], Vertices[(i + 1) % 4], Color, SDPG_World);
            PDI->DrawLine(Vertices[i + 4], Vertices[((i + 1) % 4) + 4], Color, SDPG_World);
            PDI->DrawLine(Vertices[i], Vertices[i + 4], Color, SDPG_World);
        }

        PDI->SetHitProxy(NULL);
        //delete VisProxy;
    }
}

bool FSpawnRegionVisualizer::VisProxyHandleClick(FEditorViewportClient* InViewportClient, HComponentVisProxy* VisProxy, const FViewportClick& Click)
{
    bool bEditing = false;
    if (VisProxy && VisProxy->Component.IsValid())
    {
        bEditing = true;
        
        if (ISpawnRegionInterface* SpawnRegion = GetSpawnRegion())
        {
            AActor* SpawnRegionActor = Cast<AActor>(SpawnRegion->_getUObject());
            GEditor->SelectActor(SpawnRegionActor, true, true, false, true);
        }
    }

    return bEditing;
}

bool FSpawnRegionVisualizer::GetWidgetLocation(const FEditorViewportClient* ViewportClient, FVector& OutLocation) const
{
    ISpawnRegionInterface* SpawnRegion = GetSpawnRegion();
    if (SpawnRegion != nullptr)
    {
        OutLocation = GetSpawnRegion()->GetRegionLocation();
        return true;
    }
    return false;
}

bool FSpawnRegionVisualizer::HandleInputDelta(FEditorViewportClient *ViewportClient, FViewport *Viewport, FVector &DeltaTranslate, FRotator &DeltaRotate, FVector &DeltaScale)
{
    bool bHandled = false;
    
    ISpawnRegionInterface* SpawnRegion = GetSpawnRegion();
    if (SpawnRegion != nullptr)
    {
        FVector RegionLocation = SpawnRegion->GetRegionLocation();
        SpawnRegion->SetRegionLocation(RegionLocation + DeltaTranslate);

        FRotator RegionRotation = SpawnRegion->GetRegionRotation();
        SpawnRegion->SetRegionRotation(RegionRotation + DeltaRotate);

        FVector RegionSize = SpawnRegion->GetSize();
        FVector NewSize = FVector(RegionSize.X * (1 + DeltaScale.X), RegionSize.Y * (1 + DeltaScale.Y), RegionSize.Z * (1 + DeltaScale.Z));
        SpawnRegion->SetSize(NewSize);

        bHandled = true;
    }
    
    return bHandled;
}

ISpawnRegionInterface* FSpawnRegionVisualizer::GetSpawnRegion() const
{
    UActorComponent* EditedComponent = GetEditedComponent();
    if (EditedComponent == nullptr) {return nullptr;}

    AActor* OwningActor = EditedComponent->GetOwner();
    if (OwningActor == nullptr) {return nullptr;}

    return Cast<ISpawnRegionInterface>(OwningActor);
}

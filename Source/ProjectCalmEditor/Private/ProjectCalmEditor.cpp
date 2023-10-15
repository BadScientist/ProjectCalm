#include "ProjectCalmEditor.h"
#include "SpawnRegionVisualizer.h"
#include "EditorOnly/SpawnRegionVisualizerComponent.h"


IMPLEMENT_GAME_MODULE(FProjectCalmEditorModule, ProjectCalmEditor)
DEFINE_LOG_CATEGORY(LogProjectCalmEditor)

void FProjectCalmEditorModule::StartupModule()
{
    UE_LOG(LogProjectCalmEditor, Display, TEXT("Module Startup"));

    if (GUnrealEd != NULL)
    {
        TSharedPtr<FSpawnRegionVisualizer> Visualizer = MakeShareable(new FSpawnRegionVisualizer);

        if (Visualizer.IsValid())
        {
            GUnrealEd->RegisterComponentVisualizer(USpawnRegionVisualizerComponent::StaticClass()->GetFName(), Visualizer);
            Visualizer->OnRegister();
        }
    }
}

void FProjectCalmEditorModule::ShutdownModule()
{
    UE_LOG(LogProjectCalmEditor, Display, TEXT("Module Shutdown"));
    
    if (GUnrealEd != NULL)
    {
        GUnrealEd->UnregisterComponentVisualizer(USpawnRegionVisualizerComponent::StaticClass()->GetFName());
    }
}

#undef LOCTEXT_NAMESPACE
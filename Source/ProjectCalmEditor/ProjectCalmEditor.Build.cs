// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectCalmEditor : ModuleRules
{
	public ProjectCalmEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "ProjectCalm", "UnrealEd"});
		PrivateDependencyModuleNames.AddRange(new string[] {"Core", "CoreUObject", "Engine", "ProjectCalm", "UnrealEd"});
        PublicIncludePaths.AddRange(new string[] {"ProjectCalmEditor/Public", "ProjectCalm"});
        PrivateIncludePaths.AddRange(new string[] {"ProjectCalmEditor/Private"});
	}
}

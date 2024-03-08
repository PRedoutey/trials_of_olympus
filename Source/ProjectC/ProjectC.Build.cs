// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectC : ModuleRules
{
	public ProjectC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "AIModule", "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "UMG", "Slate", "SlateCore", "Niagara", "NavigationSystem" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FlareForge : ModuleRules
{
	public FlareForge(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "GameplayTags", "GameplayAbilities", "GameplayTasks", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}

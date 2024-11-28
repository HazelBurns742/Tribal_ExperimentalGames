// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Tribal_Experimental : ModuleRules
{
	public Tribal_Experimental(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "AIModule", "Niagara", "EnhancedInput" });
    }
}

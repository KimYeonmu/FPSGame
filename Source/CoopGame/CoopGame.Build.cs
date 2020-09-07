// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class CoopGame : ModuleRules
{
	public CoopGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "NavigationSystem", "UMG" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemUtils" });

		// Uncomment if you are using Slate UI
		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");
    }
}

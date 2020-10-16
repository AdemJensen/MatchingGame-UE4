// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MatchingGame : ModuleRules
{
	public MatchingGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UnDe : ModuleRules
{
	public UnDe(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}

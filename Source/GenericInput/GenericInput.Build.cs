// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GenericInput : ModuleRules
{
	public GenericInput(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		OptimizeCode = CodeOptimization.Never;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"EnhancedInput",
				"InputCore",
				"DeveloperSettings",
				"GameplayTags",
			});

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			});
	}
}
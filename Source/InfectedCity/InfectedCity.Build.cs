// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class InfectedCity : ModuleRules
{
	public InfectedCity(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}

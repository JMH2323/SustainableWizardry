// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class SustainableWizardry : ModuleRules
{
	public SustainableWizardry(ReadOnlyTargetRules Target) : base(Target)
	{
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",
			"GameplayTasks", "GameplayAbilities", "GameplayTags", "UMG", "Niagara"
		});
	}
}

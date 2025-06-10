// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FashionDragon : ModuleRules
{
	public FashionDragon(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(["Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem"
		]);

		PrivateDependencyModuleNames.AddRange(["CADKernel"]);
		
		bUseRTTI = true;
	}
}

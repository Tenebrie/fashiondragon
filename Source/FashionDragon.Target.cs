// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FashionDragonTarget : TargetRules
{
	public FashionDragonTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("FashionDragon");
	}
}

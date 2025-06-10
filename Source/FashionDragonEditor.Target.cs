// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FashionDragonEditorTarget : TargetRules
{
	public FashionDragonEditorTarget(TargetInfo target) : base(target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
		ExtraModuleNames.Add("FashionDragon");
	}
}

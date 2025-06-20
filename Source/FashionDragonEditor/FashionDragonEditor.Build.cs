using UnrealBuildTool;

public class FashionDragonEditor : ModuleRules
{
	public FashionDragonEditor(ReadOnlyTargetRules target) : base(target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "AnimGraph", "BlueprintGraph", "UnrealEd", "FashionDragon",
		]);
		PublicIncludePaths.AddRange(["FashionDragonEditor"]);
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_6;
		
		bUseRTTI = true;
	}
}
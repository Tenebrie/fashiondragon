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
		
		bUseRTTI = true;
	}
}
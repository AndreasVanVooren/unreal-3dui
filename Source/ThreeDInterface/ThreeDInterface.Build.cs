// (c) 2021 Andreas Van Vooren

using UnrealBuildTool;

public class ThreeDInterface : ModuleRules
{
	public ThreeDInterface(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Core",
				"Engine",
				"SlateCore",
				"Slate",
				"UMG",
			}
		);
	}
}
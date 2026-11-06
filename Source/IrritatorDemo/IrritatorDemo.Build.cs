using UnrealBuildTool;

public class IrritatorDemo : ModuleRules
{
	public IrritatorDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"IrritatorRuntime"
		});
	}
}

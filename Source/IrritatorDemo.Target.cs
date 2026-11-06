using UnrealBuildTool;

public class IrritatorDemoTarget : TargetRules
{
	public IrritatorDemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("IrritatorDemo");
	}
}

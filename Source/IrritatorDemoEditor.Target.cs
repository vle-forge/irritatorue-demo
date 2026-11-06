using UnrealBuildTool;

public class IrritatorDemoEditorTarget : TargetRules
{
	public IrritatorDemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		ExtraModuleNames.Add("IrritatorDemo");
	}
}

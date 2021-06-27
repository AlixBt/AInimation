

using UnrealBuildTool;
using System.Collections.Generic;

public class AInimationTarget : TargetRules
{
	public AInimationTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		ExtraModuleNames.AddRange( new string[] { "AInimation" } );
	}
}

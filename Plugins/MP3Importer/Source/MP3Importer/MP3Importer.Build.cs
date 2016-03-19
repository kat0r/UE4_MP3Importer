// Distributed under the MIT License (MIT) (See accompanying file LICENSE
// or copy at http://opensource.org/licenses/MIT)

using UnrealBuildTool;

public class MP3Importer : ModuleRules
{
	public MP3Importer(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UnrealEd"});
		
		PrivateIncludePaths.AddRange(new string[] { "MP3Importer/Private" });
        PublicIncludePaths.AddRange(new string[] { "MP3Importer/Public" });
	}
}

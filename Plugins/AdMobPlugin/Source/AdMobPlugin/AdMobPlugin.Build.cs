using UnrealBuildTool;
using System.IO;

public class AdMobPlugin : ModuleRules
{
    public AdMobPlugin(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.Add("Launch");

            string PluginPath = Utils.MakePathRelativeTo(
                ModuleDirectory,
                Target.RelativeEnginePath
            );

            AdditionalPropertiesForReceipt.Add(
                "AndroidPlugin",
                Path.Combine(
                    PluginPath,
                    "..",
                    "..",
                    "Resources",
                    "AdMobPlugin_UPL.xml"
                )
            );
        }
    }
}

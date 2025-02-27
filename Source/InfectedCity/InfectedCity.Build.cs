using System.IO;
using UnrealBuildTool;

public class InfectedCity : ModuleRules
{
    public InfectedCity(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[]
        {
            Path.Combine(ModuleDirectory, "Public"),
            Path.Combine(ModuleDirectory, "Public/Interactable"),
            Path.Combine(ModuleDirectory, "Public/Interactable/Light"),
            Path.Combine(ModuleDirectory, "Public/InteractManager"),
        });

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput",
            "NavigationSystem"
        });
    }
}
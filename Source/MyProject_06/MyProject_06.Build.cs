using UnrealBuildTool;

public class MyProject_06 : ModuleRules
{
	public MyProject_06(ReadOnlyTargetRules Target) : base(Target)
	{


        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;


        PrivateIncludePaths.Add(ModuleDirectory);


        PublicDependencyModuleNames.Add("Core");

        PrivateDependencyModuleNames.Add("CoreUObject");
        PrivateDependencyModuleNames.Add("Engine");
        PrivateDependencyModuleNames.Add("InputCore");
        PublicDependencyModuleNames.Add("Niagara");
        PrivateDependencyModuleNames.Add("UMG");
        PrivateDependencyModuleNames.Add("CableComponent");

        PrivateDependencyModuleNames.Add("AIModule");
        PrivateDependencyModuleNames.Add("GameplayTasks");
        PrivateDependencyModuleNames.Add("NavigationSystem");
        PrivateDependencyModuleNames.Add("ProceduralMeshComponent");
    }
}

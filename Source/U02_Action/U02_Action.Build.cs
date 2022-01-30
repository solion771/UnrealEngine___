using UnrealBuildTool;

public class U02_Action : ModuleRules
{
	public U02_Action(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"AIModule",
			"GameplayTasks"
		});

		PublicIncludePaths.Add(ModuleDirectory);
	}
}

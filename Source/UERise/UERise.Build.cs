// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class UERise : ModuleRules
{
	public UERise(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicIncludePaths.AddRange(new string[] { "UERise" });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Slate", "AGRPRO",
            "SlateCore", "Engine", "InputCore", "UMG", "HeadMountedDisplay", "EnhancedInput",  "NavigationSystem", "AIModule", "GameplayTasks"});

    }                

                
}

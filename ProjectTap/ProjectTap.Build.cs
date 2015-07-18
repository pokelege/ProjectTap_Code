// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class ProjectTap : ModuleRules
{
	public ProjectTap(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", 
                                                            "CoreUObject", 
                                                            "Engine", 
                                                            "InputCore",
                                                            "AIModule", 
                                                            "UMG",
                                                            "MovieSceneCore" });

        PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
	}
}

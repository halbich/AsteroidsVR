// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class AsteroidsVR : ModuleRules
{
	public AsteroidsVR(TargetInfo Target)
	{
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "UMG", "KinectModule" });

        PublicIncludePaths.AddRange(new string[] { "KinectModule/Public" });
    }
}

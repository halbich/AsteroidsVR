// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.IO;

public class KinectModule : ModuleRules
{
    public KinectModule(TargetInfo Target)
    {
        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });

        LoadKinectLibrary(Target);
    }


    public bool LoadKinectLibrary(TargetInfo Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "KinectSDK", "Libraries");

            var publicAdd = Path.Combine(LibrariesPath, PlatformString, "Kinect20.lib");

            //test your path with:
            //using System; // Console.WriteLine("");
            System.Console.WriteLine("... LibrariesPath -> " + publicAdd);


            PublicAdditionalLibraries.Add(publicAdd);
        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "KinectSDK", "Includes"));
        }

        Definitions.Add(string.Format("WITH_KINECT_SDK_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }

    private string ModulePath
    {
        get
        {
            return ModuleDirectory;
        }
    }

    private string ThirdPartyPath
    {
        get
        {
            return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty"));
        }
    }
}

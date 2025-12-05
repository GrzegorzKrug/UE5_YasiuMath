/*
 * Copyright (c) 2025 Grzegorz Krug.
 * All Rights Reserved.
 */

using UnrealBuildTool;
using System.IO;


public class YasiuMath : ModuleRules
{
	public bool bStrictIncludesCheck = false;

	public YasiuMath(ReadOnlyTargetRules Target) : base(Target)
	{
		CppStandard = CppStandardVersion.Cpp20;
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		if (bStrictIncludesCheck)
		{
			bUseUnity = false;
			PCHUsage = PCHUsageMode.NoPCHs;

			// Enable additional checks used for Engine modules
			bTreatAsEngineModule = true;
		}
		// bUseUnity = false;
		// PCHUsage = ModuleRules.PCHUsageMode.NoPCHs;
		// bTreatAsEngineModule = true;


		PublicIncludePaths.AddRange(
			new string[]
			{
				// Path.Combine(ModuleDirectory, "Public"),
				// ... add public include paths required here ...
				// "SquirrelRNG.cpp"
			}
		);
		PrivateIncludePaths.AddRange(
			new string[]
			{
				// Path.Combine(ModuleDirectory, "Private"),
				// ... add other private include paths required here ...
				// "YasiuMath/Public",
				// "YasiuMathStaticLib.cpp"
			}
		);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				// ... add other public dependencies that you statically link with here ...
			}
		);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				// "Slate",
				// "SlateCore",
				// ... add private dependencies that you statically link with here ...	
			}
		);


		// DynamicallyLoadedModuleNames.AddRange(
		// 	new string[]
		// 	{
		// 		// ... add any modules that your module loads dynamically here ...
		// 	}
		// );
	}
}
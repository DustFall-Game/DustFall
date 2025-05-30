// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DustFall : ModuleRules
{
	public DustFall(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG", "PhysicsCore", "AnimGraphRuntime" });

		PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystem", "OnlineSubsystemSteam" });
		
	}
}

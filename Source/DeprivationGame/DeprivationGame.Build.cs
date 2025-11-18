// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class DeprivationGame : ModuleRules
{
	public DeprivationGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "ChaosVehicles", "UMG", "Slate", "SlateCore", "GameplayTasks" });

		PrivateDependencyModuleNames.AddRange(new string[] {  });
		
		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
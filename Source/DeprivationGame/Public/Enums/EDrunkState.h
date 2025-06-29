#pragma once

UENUM(Blueprintable)
enum class EDrunkState:uint8
{
	Sober UMETA(DisplayName = "Sober"), 
	Tipsy UMETA(DisplayName = "Tipsy"), 
	Drunk UMETA(DisplayName = "Drunk"), 
	Wasted UMETA(DisplayName = "Wasted")
};

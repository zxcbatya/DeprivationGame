#pragma once

UENUM(BlueprintType)
enum class EFatigueState:uint8
{
	Fresh UMETA(DisplayName = "Fresh"),
	Tired UMETA(DisplayName = "Tired"),
	Exhausted UMETA(DisplayName = "Exhausted")
};
Ы
#pragma once

#include "CoreMinimal.h"
#include "EFatigueState.generated.h"

UENUM(BlueprintType)
enum class EFatigueState : uint8
{
    Rested    UMETA(DisplayName = "Rested"),
    Tired     UMETA(DisplayName = "Tired"),
    Exhausted UMETA(DisplayName = "Exhausted"),
    Sleepy    UMETA(DisplayName = "Sleepy"),
    Collapsed UMETA(DisplayName = "Collapsed")
};

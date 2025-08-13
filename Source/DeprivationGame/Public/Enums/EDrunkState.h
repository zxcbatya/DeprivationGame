#pragma once

#include "CoreMinimal.h"
#include "EDrunkState.generated.h"

UENUM(BlueprintType)
enum class EDrunkState : uint8
{
    Sober     UMETA(DisplayName = "Sober"),
    Tipsy     UMETA(DisplayName = "Tipsy"),
    Drunk     UMETA(DisplayName = "Drunk"),
    VeryDrunk UMETA(DisplayName = "Very Drunk"),
    Blackout  UMETA(DisplayName = "Blackout")
};

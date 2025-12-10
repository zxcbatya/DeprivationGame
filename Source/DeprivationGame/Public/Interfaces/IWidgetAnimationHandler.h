#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWidgetAnimationHandler.generated.h"

UINTERFACE(BlueprintType)
class DEPRIVATIONGAME_API UWidgetAnimationHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * Interface for handling widget animations via C++ interface calls rather than Blueprint events
 */
class DEPRIVATIONGAME_API IWidgetAnimationHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI Animation")
	void PlayShowAnimation();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "UI Animation")
	void PlayHideAnimation();
};
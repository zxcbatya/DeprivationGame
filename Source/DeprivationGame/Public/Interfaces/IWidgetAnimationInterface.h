#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IWidgetAnimationInterface.generated.h"

UINTERFACE(BlueprintType)
class DEPRIVATIONGAME_API UWidgetAnimationInterface : public UInterface
{
	GENERATED_BODY()
};

class DEPRIVATIONGAME_API IWidgetAnimationInterface
{
	GENERATED_BODY()

public:
	/**
	 * Воспроизводит анимацию появления виджета
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget Animation")
	void PlayShowAnimation();

	/**
	 * Воспроизводит анимацию скрытия виджета
	 */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Widget Animation")
	void PlayHideAnimation();
};
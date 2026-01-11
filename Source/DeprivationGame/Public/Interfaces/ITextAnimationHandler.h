#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ITextAnimationHandler.generated.h"

UINTERFACE(BlueprintType)
class DEPRIVATIONGAME_API UTextAnimationHandler : public UInterface
{
	GENERATED_BODY()
};

class DEPRIVATIONGAME_API ITextAnimationHandler
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Text Animation")
	void AnimateText(const FString& Text);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Text Animation")
	void StopTextAnimation();
};
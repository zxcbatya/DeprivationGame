#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IChopWoodGameListener.generated.h"

UINTERFACE(BlueprintType)
class DEPRIVATIONGAME_API UChopWoodGameListener : public UInterface
{
	GENERATED_BODY()
};

class DEPRIVATIONGAME_API IChopWoodGameListener
{
	GENERATED_BODY()

public:
	// Game completion events
	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood")
	void OnChopWoodGameCompleted();

	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood")
	void OnChopWoodGameFailed();
    
	// Hit events
	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood")
	void OnChopWoodHitSuccess(int32 HitCount);
    
	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood")
	void OnChopWoodHitFailed(int32 FailCount);

	// Animation events - these will be called from the widget to trigger character animations
	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood|Animation")
	void PlayChopStartAnimation();

	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood|Animation")
	void PlayChopSuccessAnimation();

	UFUNCTION(BlueprintNativeEvent, Category = "ChopWood|Animation")
	void PlayChopFailAnimation();
};
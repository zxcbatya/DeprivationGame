#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/Image.h"
#include "ChopWoodWidget.generated.h"

class UImage;
class UProgressBar;
class UCanvasPanel;

UCLASS()
class DEPRIVATIONGAME_API UChopWoodWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;
	
	UPROPERTY(meta = (BindWidget))
	UImage* TargetZoneImage;
	
	UPROPERTY(meta = (BindWidget))
	UImage* IndicatorImage;
	
	float CurrentTargetZoneCenter = 0.5f;
	float CurrentTargetZoneSize = 0.2f;
	float CurrentIndicatorPosition = 0.0f;

public:
	UChopWoodWidget(const FObjectInitializer& ObjectInitializer);
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void UpdateVisuals(float InTargetZoneCenter, float InTargetZoneSize, float InIndicatorPosition);

	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlaySuccessAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlayFailureAnimation();


};

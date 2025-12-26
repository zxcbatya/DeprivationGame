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
	UImage* TargetZoneImage;

	UPROPERTY(meta = (BindWidget))
	UImage* IndicatorImage;

	UPROPERTY(meta = (BindWidget))
	UImage* BackgroundImage;

	// Positioning parameters that can be adjusted in editor
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float TopPadding = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float BottomPadding = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float TopVerticalPadding = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float BottomVerticalPadding = 50.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float TargetZoneVerticalOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float IndicatorVerticalOffset = 0.0f;

	// Internal variables to store current state
	float CurrentTargetZoneCenter = 0.5f;
	float CurrentTargetZoneSize = 0.2f;
	float CurrentIndicatorPosition = 0.0f;

public:
	UChopWoodWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void UpdateVisuals(float InTargetZoneCenter, float InCurrentTargetZoneSize, float InIndicatorPosition);

	// Animations that can be implemented in Blueprint
	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlaySuccessAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlayFailureAnimation();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};

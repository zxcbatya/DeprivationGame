#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float TargetZoneSize = 0.2f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float IndicatorPosition;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	bool IndicatorDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float CurrentTargetZoneSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood")
	float TargetZoneCenter = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float BackgroundOffsetX = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float BackgroundOffsetY = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float ManualBackgroundWidth = 0.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float TopPadding = 120.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float BottomPadding = 120.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float TargetZoneVerticalOffset = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ChopWood|Positioning")
	float IndicatorVerticalOffset = 0.0f;

public:
	UChopWoodWidget(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "ChopWood")
	void UpdateVisuals();

	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlaySuccessAnimation();

	UFUNCTION(BlueprintImplementableEvent, Category = "ChopWood")
	void PlayFailureAnimation();

protected:
	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
};
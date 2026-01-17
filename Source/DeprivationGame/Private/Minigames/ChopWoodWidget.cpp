#include "Minigames/ChopWoodWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include "Engine/Texture2D.h"

UChopWoodWidget::UChopWoodWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	CurrentTargetZoneCenter = 0.5f;
	CurrentTargetZoneSize = 0.2f;
	CurrentIndicatorPosition = 0.0f;
}

void UChopWoodWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChopWoodWidget::UpdateVisuals(float InTargetZoneCenter, float InTargetZoneSize, float InIndicatorPosition)
{
	UCanvasPanel* RootPanel = Cast<UCanvasPanel>(GetRootWidget());
	FVector2D PanelSize = RootPanel->GetDesiredSize();

	const float PaddingLeft = 120.0f;
	const float PaddingRight = 0.0f;
	const float PaddingTop = 0.0f;
	const float PaddingBottom = 50.0f;

	float WorkAreaWidth = PanelSize.X - PaddingLeft - PaddingRight;
	float WorkAreaHeight = PanelSize.Y - PaddingTop - PaddingBottom;

	float WorkAreaX = PaddingLeft;
	float WorkAreaY = PaddingTop;
	float WorkAreaCenterY = WorkAreaY + WorkAreaHeight * 0.5f;

	if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
	{
		float ZoneWidth = FMath::Max(1.0f, InTargetZoneSize * WorkAreaWidth);
		float ZoneX = WorkAreaX + InTargetZoneCenter * WorkAreaWidth - ZoneWidth * 0.5f;

		ZoneSlot->SetPosition(FVector2D(ZoneX, WorkAreaCenterY - 22.5f));
		ZoneSlot->SetSize(FVector2D(ZoneWidth, 45.0f));
		ZoneSlot->SetZOrder(1);
	}

	if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
	{
		const float IndicatorWidth = 50.0f;
		const float IndicatorHeight = 45.0f;
		float IndicatorX = WorkAreaX + InIndicatorPosition * WorkAreaWidth - IndicatorWidth * 0.5f;

		IndicatorSlot->SetPosition(FVector2D(IndicatorX, WorkAreaCenterY - IndicatorHeight * 0.5f));
		IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
		IndicatorSlot->SetZOrder(2);
	}
}
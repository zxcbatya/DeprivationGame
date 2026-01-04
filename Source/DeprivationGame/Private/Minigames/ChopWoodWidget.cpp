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
	if (!BackgroundImage || !TargetZoneImage || !IndicatorImage) return;

	FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
	FVector2D BackgroundPos = BackgroundGeom.GetAbsolutePosition();
	FVector2D BackgroundSize = BackgroundGeom.GetAbsoluteSize();

	float UsableWidth = BackgroundSize.X;
	float CenterY = BackgroundPos.Y + (BackgroundSize.Y * 0.5f);

	if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
	{
		float ZoneWidth = FMath::Max(1.0f, InTargetZoneSize * UsableWidth);
		float ZoneHeight = 45.0f;
		float ZoneX = BackgroundPos.X + (InTargetZoneCenter * UsableWidth) - (ZoneWidth * 0.5f);

		ZoneSlot->SetPosition(FVector2D(ZoneX, CenterY));
		ZoneSlot->SetSize(FVector2D(ZoneWidth, ZoneHeight));
		ZoneSlot->SetZOrder(1);
		TargetZoneImage->InvalidateLayoutAndVolatility();
	}

	if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
	{
		float IndicatorWidth = 50.0f;
		float IndicatorHeight = 45.0f;
		float IndicatorX = BackgroundPos.X + (InIndicatorPosition * UsableWidth) - (IndicatorWidth * 0.5f);

		IndicatorSlot->SetPosition(FVector2D(IndicatorX, CenterY));
		IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
		IndicatorSlot->SetZOrder(2);
		IndicatorImage->InvalidateLayoutAndVolatility();
	}
}
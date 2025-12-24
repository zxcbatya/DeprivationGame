#include "Minigames/ChopWoodWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UChopWoodWidget::UChopWoodWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	IndicatorPosition = 0.0f;
	IndicatorDirection = true;
	CurrentTargetZoneSize = 0.2f;
	TargetZoneCenter = 0.5f;
	BackgroundOffsetX = 0.0f;
	BackgroundOffsetY = 0.0f;
	ManualBackgroundWidth = 0.0f;
	TopPadding = 300.0f;
	BottomPadding = 120.0f;
}

void UChopWoodWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChopWoodWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	UpdateVisuals();
}

void UChopWoodWidget::UpdateVisuals()
{
	FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
	FVector2D BackgroundSize = BackgroundGeom.GetLocalSize();

	float EffectiveBackgroundWidth = (ManualBackgroundWidth > 0.0f) ? ManualBackgroundWidth : BackgroundSize.X;

	float UsableWidth = EffectiveBackgroundWidth - TopPadding - BottomPadding;

	float StartX = TopPadding;

	if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
	{
		const float ZoneWidthPixels = CurrentTargetZoneSize * UsableWidth;
		const float ZoneHeightPixels = 45.0f;

		float ZoneX = StartX + (TargetZoneCenter * UsableWidth) - (ZoneWidthPixels * 0.5f);
		float ZoneY = (BackgroundSize.Y - ZoneHeightPixels) * 0.5f + TargetZoneVerticalOffset;

		ZoneX = FMath::Clamp(ZoneX, StartX, StartX + UsableWidth - ZoneWidthPixels);

		ZoneSlot->SetSize(FVector2D(ZoneWidthPixels, ZoneHeightPixels));
		ZoneSlot->SetPosition(FVector2D(ZoneX, ZoneY));

		TargetZoneImage->InvalidateLayoutAndVolatility();
	}

	if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
	{
		float Pos = FMath::Clamp(IndicatorPosition, 0.0f, 1.0f);
		const float IndicatorWidth = 50.0f;
		const float IndicatorHeight = 45.0f;

		float IndicatorX = StartX + (Pos * UsableWidth) - (IndicatorWidth * 0.5f);
		float IndicatorY = (BackgroundSize.Y - IndicatorHeight) * 0.5f + IndicatorVerticalOffset;

		IndicatorX = FMath::Clamp(IndicatorX, StartX, StartX + UsableWidth - IndicatorWidth);

		IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
		IndicatorSlot->SetPosition(FVector2D(IndicatorX, IndicatorY));

		IndicatorImage->InvalidateLayoutAndVolatility();
	}
}

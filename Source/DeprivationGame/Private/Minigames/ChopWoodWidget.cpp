#include "Minigames/ChopWoodWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UChopWoodWidget::UChopWoodWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), TargetZoneImage(nullptr), IndicatorImage(nullptr), BackgroundImage(nullptr)
{
	TopPadding = 120.0f;
	BottomPadding = 120.0f;
	BottomVerticalPadding = 50.0f;
	TopVerticalPadding = 0.0f;
	TargetZoneVerticalOffset = 0.0f;
	IndicatorVerticalOffset = 0.0f;
}

void UChopWoodWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UChopWoodWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	// Update visuals using stored values
	UpdateVisuals(CurrentTargetZoneCenter, CurrentTargetZoneSize, CurrentIndicatorPosition);
}

void UChopWoodWidget::UpdateVisuals(float InTargetZoneCenter, float InCurrentTargetZoneSize, float InIndicatorPosition)
{
    // Store the values for potential use with the parameterless version
    CurrentTargetZoneCenter = InTargetZoneCenter;
    CurrentTargetZoneSize = InCurrentTargetZoneSize;
    CurrentIndicatorPosition = InIndicatorPosition;

    if (!BackgroundImage || !TargetZoneImage || !IndicatorImage)
        return;

    FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
    FVector2D BackgroundSize = BackgroundGeom.GetLocalSize();

    float UsableWidth = BackgroundSize.X - (TopPadding + BottomPadding);
    float StartX = TopPadding;

    float CenterY = (BackgroundSize.Y * 0.5f) - (BottomVerticalPadding * 0.5f) + (TopVerticalPadding * 0.5f);

    auto PositionElement = [&](float PositionRatio, float ElementWidth, float ElementHeight, float VerticalOffset) -> FVector2D
    {
        float ElementX = StartX + (PositionRatio * UsableWidth) - (ElementWidth * 0.5f);
        float ElementY = CenterY + VerticalOffset;

        ElementX = FMath::Clamp(ElementX, StartX, StartX + UsableWidth - ElementWidth);
        
        float MinY = TopVerticalPadding;
        float MaxY = BackgroundSize.Y - BottomVerticalPadding - ElementHeight;
        ElementY = FMath::Clamp(ElementY, MinY, MaxY);

        return FVector2D(ElementX, ElementY);
    };

    if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
    {
        float ZoneWidth = InCurrentTargetZoneSize * UsableWidth;
        float ZoneHeight = 45.0f;

        FVector2D ZonePos = PositionElement(InTargetZoneCenter, ZoneWidth, ZoneHeight, TargetZoneVerticalOffset);

        ZoneSlot->SetSize(FVector2D(ZoneWidth, ZoneHeight));
        ZoneSlot->SetPosition(ZonePos);
        TargetZoneImage->InvalidateLayoutAndVolatility();
    }

    if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
    {
        float IndicatorWidth = 50.0f;
        float IndicatorHeight = 45.0f;
        float Pos = FMath::Clamp(InIndicatorPosition, 0.0f, 1.0f);

        FVector2D IndicatorPos = PositionElement(Pos, IndicatorWidth, IndicatorHeight, IndicatorVerticalOffset);

        IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
        IndicatorSlot->SetPosition(IndicatorPos);
        IndicatorImage->InvalidateLayoutAndVolatility();
    }
}
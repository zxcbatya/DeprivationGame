#include "Minigames/ChopWoodWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"

UChopWoodWidget::UChopWoodWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer), TargetZoneImage(nullptr), IndicatorImage(nullptr), BackgroundImage(nullptr)
{
	IndicatorPosition = 0.0f;
	IndicatorDirection = true;
	CurrentTargetZoneSize = 0.2f;
	TargetZoneCenter = 0.5f;
	BackgroundOffsetX = 0.0f;
	BackgroundOffsetY = 0.0f;
	ManualBackgroundWidth = 0.0f;
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
	UpdateVisuals();
}

void UChopWoodWidget::UpdateVisuals()
{
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
        float ZoneWidth = CurrentTargetZoneSize * UsableWidth;
        float ZoneHeight = 45.0f;

        FVector2D ZonePos = PositionElement(TargetZoneCenter, ZoneWidth, ZoneHeight, TargetZoneVerticalOffset);

        ZoneSlot->SetSize(FVector2D(ZoneWidth, ZoneHeight));
        ZoneSlot->SetPosition(ZonePos);
        TargetZoneImage->InvalidateLayoutAndVolatility();
    }

    if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
    {
        float IndicatorWidth = 50.0f;
        float IndicatorHeight = 45.0f;
        float Pos = FMath::Clamp(IndicatorPosition, 0.0f, 1.0f);

        FVector2D IndicatorPos = PositionElement(Pos, IndicatorWidth, IndicatorHeight, IndicatorVerticalOffset);

        IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
        IndicatorSlot->SetPosition(IndicatorPos);
        IndicatorImage->InvalidateLayoutAndVolatility();
    }
}

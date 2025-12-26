#include "Minigames/ChopWoodWidget.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/Image.h"
#include "Components/CanvasPanel.h"

UChopWoodWidget::UChopWoodWidget(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	// Не инициализируем указатели здесь, они будут установлены в Construct()
	TopPadding = 120.0f;
	BottomPadding = 120.0f;
	BottomVerticalPadding = 50.0f;
	TopVerticalPadding = 0.0f;
	TargetZoneVerticalOffset = 0.0f;
	IndicatorVerticalOffset = 0.0f;

	// Устанавливаем значения по умолчанию для визуальных параметров
	CurrentTargetZoneCenter = 0.5f;
	CurrentTargetZoneSize = 0.2f;
	CurrentIndicatorPosition = 0.0f;
}

void UChopWoodWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!BackgroundImage)
	{
		BackgroundImage = Cast<UImage>(GetWidgetFromName(TEXT("BackgroundImage")));
	}
	if (!TargetZoneImage)
	{
		TargetZoneImage = Cast<UImage>(GetWidgetFromName(TEXT("TargetZoneImage")));
	}
	if (!IndicatorImage)
	{
		IndicatorImage = Cast<UImage>(GetWidgetFromName(TEXT("IndicatorImage")));
	}

	if (!BackgroundImage || !TargetZoneImage || !IndicatorImage)
	{
		if (UCanvasPanel* CanvasPanel = Cast<UCanvasPanel>(GetRootWidget()))
		{
			for (int32 i = 0; i < CanvasPanel->GetChildrenCount(); ++i)
			{
				UWidget* Child = CanvasPanel->GetChildAt(i);
				if (Child->GetName().Contains("BackgroundImage"))
				{
					BackgroundImage = Cast<UImage>(Child);
				}
				else if (Child->GetName().Contains("TargetZoneImage"))
				{
					TargetZoneImage = Cast<UImage>(Child);
				}
				else if (Child->GetName().Contains("IndicatorImage"))
				{
					IndicatorImage = Cast<UImage>(Child);
				}
			}
		}
	}
}

void UChopWoodWidget::UpdateVisuals(float InTargetZoneCenter, float InTargetZoneSize, float InIndicatorPosition)
{
	// Сохраняем значения для потенциального использования
	CurrentTargetZoneCenter = InTargetZoneCenter;
	CurrentTargetZoneSize = InTargetZoneSize;
	CurrentIndicatorPosition = InIndicatorPosition;

	// Проверка на nullptr
	if (!BackgroundImage || !TargetZoneImage || !IndicatorImage)
	{
		UE_LOG(LogTemp, Warning, TEXT("Cannot update visuals: one or more images are null."));
		return;
	}

	// Получаем геометрию фона
	FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
	FVector2D BackgroundAbsolutePos = BackgroundGeom.GetAbsolutePosition();
	FVector2D BackgroundAbsoluteSize = BackgroundGeom.GetAbsoluteSize();

	// Рассчитываем используемую ширину
	float UsableWidth = BackgroundAbsoluteSize.X - (TopPadding + BottomPadding);
	float StartX = BackgroundAbsolutePos.X + TopPadding;

	// Центр по Y для элементов
	float ElementsY = BackgroundAbsolutePos.Y + (BackgroundAbsoluteSize.Y * 0.5f);

	// Обновляем TargetZoneImage
	if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
	{
		float ZoneWidth = FMath::Max(1.0f, InTargetZoneSize * UsableWidth);
		float ZoneHeight = 45.0f;
		float ZoneX = StartX + (InTargetZoneCenter * UsableWidth) - (ZoneWidth * 0.5f);

		ZoneSlot->SetPosition(FVector2D(ZoneX, ElementsY + TargetZoneVerticalOffset));
		ZoneSlot->SetSize(FVector2D(ZoneWidth, ZoneHeight));
		ZoneSlot->SetZOrder(1); // Поверх фона
		TargetZoneImage->InvalidateLayoutAndVolatility();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("TargetZoneImage slot is not a CanvasPanelSlot."));
	}

	// Обновляем IndicatorImage
	if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
	{
		float IndicatorWidth = 50.0f;
		float IndicatorHeight = 45.0f;
		float IndicatorX = StartX + (InIndicatorPosition * UsableWidth) - (IndicatorWidth * 0.5f);

		IndicatorSlot->SetPosition(FVector2D(IndicatorX, ElementsY + IndicatorVerticalOffset));
		IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
		IndicatorSlot->SetZOrder(1); // Поверх фона
		IndicatorImage->InvalidateLayoutAndVolatility();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("IndicatorImage slot is not a CanvasPanelSlot."));
	}
}
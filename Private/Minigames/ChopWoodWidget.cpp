// Fill out your copyright notice in the Description page of Project Settings.

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
	BackgroundOffsetX = 0.0f;  // Инициализируем нулем, будем вычислять динамически
	BackgroundOffsetY = 0.0f;
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

FVector2D UChopWoodWidget::GetBackgroundRealSize() const
{
	if (BackgroundImage)
	{
		FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
		return BackgroundGeom.GetLocalSize();
	}
	return FVector2D(0.0f, 0.0f);
}

FVector2D UChopWoodWidget::GetBackgroundRealOffset() const
{
	if (BackgroundImage)
	{
		FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
		return BackgroundGeom.GetAbsolutePosition();
	}
	return FVector2D(0.0f, 0.0f);
}

void UChopWoodWidget::UpdateVisuals()
{
    // Получаем геометрию фонового изображения
    FGeometry BackgroundGeom = BackgroundImage->GetCachedGeometry();
    FVector2D BackgroundSize = BackgroundGeom.GetLocalSize();
    
    // Получаем позицию фонового изображения относительно виджета
    FVector2D BackgroundPosition = BackgroundGeom.GetAbsolutePosition();

    const float ElementPadding = 120.0f; 
    const float EffectiveBackgroundWidth = BackgroundSize.X - 2 * ElementPadding/3.0f;

    // ==== Target Zone (Зелёная зона) ====
    if (UCanvasPanelSlot* ZoneSlot = Cast<UCanvasPanelSlot>(TargetZoneImage->Slot))
    {
        const float ZoneWidthPixels = CurrentTargetZoneSize * EffectiveBackgroundWidth;
        const float ZoneHeightPixels = 45.0f;
        
        float ZoneX = (TargetZoneCenter * EffectiveBackgroundWidth) - (ZoneWidthPixels * 0.5f) + BackgroundPosition.X + ElementPadding;
        float ZoneY = -25.0f + BackgroundPosition.Y;
        
        ZoneX = FMath::Clamp(ZoneX, BackgroundPosition.X + ElementPadding, BackgroundPosition.X + BackgroundSize.X - ZoneWidthPixels - ElementPadding);
        
        ZoneSlot->SetSize(FVector2D(ZoneWidthPixels, ZoneHeightPixels));
        ZoneSlot->SetPosition(FVector2D(ZoneX, ZoneY));
        
        TargetZoneImage->InvalidateLayoutAndVolatility();
    }

    // ==== Indicator ====
    if (UCanvasPanelSlot* IndicatorSlot = Cast<UCanvasPanelSlot>(IndicatorImage->Slot))
    {
        float Pos = FMath::Clamp(IndicatorPosition, 0.0f, 1.0f);

        const float IndicatorWidth = 100.0f;
        const float IndicatorHeight = 45.0f;

        float IndicatorX = Pos * EffectiveBackgroundWidth - IndicatorWidth * 0.5f + BackgroundPosition.X + ElementPadding;
        float IndicatorY = -25.0f + BackgroundPosition.Y;

        IndicatorX = FMath::Clamp(IndicatorX, BackgroundPosition.X + ElementPadding, BackgroundPosition.X + BackgroundSize.X - IndicatorWidth - ElementPadding);
        IndicatorY = -25.0f + BackgroundPosition.Y;

        IndicatorSlot->SetSize(FVector2D(IndicatorWidth, IndicatorHeight));
        IndicatorSlot->SetPosition(FVector2D(IndicatorX, IndicatorY));

        IndicatorImage->InvalidateLayoutAndVolatility();
    }
}

void UChopWoodWidget::PlaySuccessAnimation()
{
	// Здесь можно добавить логику анимации успешного удара
	// Например, изменить цвет индикатора или добавить визуальный эффект
}

void UChopWoodWidget::PlayFailureAnimation()
{
	// Здесь можно добавить логику анимации неудачного удара
	// Например, покраснение индикатора или вибрацию
}
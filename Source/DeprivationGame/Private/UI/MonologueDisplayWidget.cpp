#include "UI/MonologueDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UMonologueDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Начальная прозрачность для анимации появления
	SetRenderOpacity(0.0f);
	
	if (GetWorld())
	{
		// Плавное появление за 0.2 секунды
		GetWorld()->GetTimerManager().SetTimer(
			FadeInTimerHandle,
			this,
			&UMonologueDisplayWidget::FadeInWidget,
			0.02f,
			true
		);
	}
}

void UMonologueDisplayWidget::NativeDestruct()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(FadeInTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
	}
	Super::NativeDestruct();
}

void UMonologueDisplayWidget::FadeInWidget()
{
	float CurrentOpacity = GetRenderOpacity();
	CurrentOpacity += 0.1f; // 0.2 секунды / 0.02 интервал = 10 шагов
	
	if (CurrentOpacity >= 1.0f)
	{
		SetRenderOpacity(1.0f);
		if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(FadeInTimerHandle);
	}
	else
	{
		SetRenderOpacity(CurrentOpacity);
	}
}

void UMonologueDisplayWidget::FadeOutWidget()
{
	float CurrentOpacity = GetRenderOpacity();
	CurrentOpacity -= 0.1f;
	
	if (CurrentOpacity <= 0.0f)
	{
		SetRenderOpacity(0.0f);
		if (GetWorld()) GetWorld()->GetTimerManager().ClearTimer(FadeOutTimerHandle);
		RemoveFromParent();
	}
	else
	{
		SetRenderOpacity(CurrentOpacity);
	}
}

void UMonologueDisplayWidget::AnimateText(const FString& Text, const FString& CharacterName)
{
	if (!MonologueText) return;
	
	// Устанавливаем имя персонажа
	if (CharacterNameText)
	{
		CharacterNameText->SetText(FText::FromString(CharacterName));
	}
	
	TargetText = Text;
	CurrentCharIndex = 0;
	MonologueText->SetText(FText::GetEmpty());
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			AnimationTimerHandle,
			[this]()
			{
				if (CurrentCharIndex < TargetText.Len())
				{
					FString DisplayText = TargetText.Left(CurrentCharIndex + 1);
					MonologueText->SetText(FText::FromString(DisplayText));
					CurrentCharIndex++;
				}
				else
				{
					GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
				}
			},
			AnimationSpeed,
			true
		);
		
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			this,
			&UMonologueDisplayWidget::HideWidget,
			DisplayDuration,
			false
		);
	}
}

void UMonologueDisplayWidget::SetMonologueText(const FString& Text, const FString& CharacterName)
{
	if (CharacterNameText)
	{
		CharacterNameText->SetText(FText::FromString(CharacterName));
	}
	
	if (MonologueText)
	{
		MonologueText->SetText(FText::FromString(Text));
	}
}

void UMonologueDisplayWidget::HideWidget()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
		
		// Плавное исчезновение
		GetWorld()->GetTimerManager().SetTimer(
			FadeOutTimerHandle,
			this,
			&UMonologueDisplayWidget::FadeOutWidget,
			0.02f,
			true
		);
	}
}
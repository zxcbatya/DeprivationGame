#include "UI/MonologueDisplayWidget.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"

void UMonologueDisplayWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMonologueDisplayWidget::NativeDestruct()
{
	StopTextAnimation_Implementation();
	Super::NativeDestruct();
}

void UMonologueDisplayWidget::AnimateText_Implementation(const FString& Text)
{
	if (!MonologueText) return;

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

void UMonologueDisplayWidget::StopTextAnimation_Implementation()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(AnimationTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}
}

void UMonologueDisplayWidget::SetMonologueText(const FString& Text)
{
	if (MonologueText)
	{
		MonologueText->SetText(FText::FromString(Text));
	}
}

void UMonologueDisplayWidget::HideWidget()
{
	StopTextAnimation_Implementation();
	RemoveFromParent();
}
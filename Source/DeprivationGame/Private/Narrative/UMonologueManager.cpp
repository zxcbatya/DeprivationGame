#include "Narrative/UMonologueManager.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Blueprint/UserWidget.h"

void UUMonologueManager::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UUMonologueManager::Deinitialize()
{
	StopCurrentMonologue();
	Super::Deinitialize();
}

void UUMonologueManager::PlayMonologue(FName MonologueID, UDataTable* MonologueTable)
{
	if (!MonologueTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("Monologue DataTable is null!"));
		return;
	}

	if (bIsPlaying)
	{
		StopCurrentMonologue();
	}

	FMonologueData* MonologueData = MonologueTable->FindRow<FMonologueData>(MonologueID, TEXT(""));
	if (!MonologueData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Monologue %s not found in DataTable!"), *MonologueID.ToString());
		return;
	}

	CurrentMonologueID = MonologueID;
	bIsPlaying = true;
	CurrentSubtitles = MonologueData->Subtitles;
	CurrentSubtitleIndex = 0;

	OnMonologueStarted.Broadcast(CurrentMonologueID);

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			this,
			&UUMonologueManager::FinishMonologue,
			MonologueData->TotalDisplayTime,
			false
		);
	}

	StartSubtitleSequence();
	UE_LOG(LogTemp, Log, TEXT("Playing monologue: %s"), *MonologueID.ToString());
}

void UUMonologueManager::StopCurrentMonologue()
{
	if (!bIsPlaying) return;

	bIsPlaying = false;

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SubtitleTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}

	OnMonologueEnded.Broadcast(CurrentMonologueID);
	CurrentMonologueID = NAME_None;
	CurrentSubtitles.Empty();
	CurrentSubtitleIndex = 0;
}

void UUMonologueManager::StartSubtitleSequence()
{
	CreateDisplayWidget();
	
	if (CurrentSubtitles.Num() > 0 && CurrentSubtitleIndex < CurrentSubtitles.Num())
	{
		FString CurrentText = CurrentSubtitles[CurrentSubtitleIndex];
		OnSubtitleChanged.Broadcast(CurrentMonologueID, CurrentText);
		
		if (ActiveWidget)
		{
			ActiveWidget->AnimateText(CurrentText);
		}
		
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SubtitleTimerHandle,
				this,
				&UUMonologueManager::AdvanceToNextSubtitle,
				3.0f,
				false
			);
		}
	}
	else
	{
		FinishMonologue();
	}
}

void UUMonologueManager::AdvanceToNextSubtitle()
{
	CurrentSubtitleIndex++;
	
	if (CurrentSubtitleIndex < CurrentSubtitles.Num())
	{
		FString CurrentText = CurrentSubtitles[CurrentSubtitleIndex];
		OnSubtitleChanged.Broadcast(CurrentMonologueID, CurrentText);
		
		if (ActiveWidget)
		{
			ActiveWidget->AnimateText(CurrentText);
		}
		
		if (GetWorld())
		{
			GetWorld()->GetTimerManager().SetTimer(
				SubtitleTimerHandle,
				this,
				&UUMonologueManager::AdvanceToNextSubtitle,
				3.0f,
				false
			);
		}
	}
	else
	{
		FinishMonologue();
	}
}

void UUMonologueManager::FinishMonologue()
{
	StopCurrentMonologue();
}

void UUMonologueManager::HandleAudioFinished()
{
	// Обработка завершения аудио
}

void UUMonologueManager::CreateDisplayWidget()
{
	if (!MonologueWidgetClass || !GetWorld()) return;

	UUserWidget* CreatedWidget = CreateWidget(GetWorld(), MonologueWidgetClass);
	if (CreatedWidget && CreatedWidget->IsA(UMonologueDisplayWidget::StaticClass()))
	{
		ActiveWidget = Cast<UMonologueDisplayWidget>(CreatedWidget);
		if (ActiveWidget)
		{
			ActiveWidget->AddToViewport();
		}
	}
}

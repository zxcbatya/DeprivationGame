#include "Narrative/UMonologueComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerController.h"
#include "Components/AudioComponent.h"
#include "Data/FMonologueData.h"

UUMonologueComponent::UUMonologueComponent(): AudioComponent(nullptr), ActiveWidget(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UUMonologueComponent::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (PC)
	{
		AudioComponent = PC->FindComponentByClass<UAudioComponent>();
		if (!AudioComponent)
		{
			AudioComponent = NewObject<UAudioComponent>(PC);
			AudioComponent->RegisterComponent();
		}
	}
}

void UUMonologueComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	StopCurrentMonologue();
	Super::EndPlay(EndPlayReason);
}

void UUMonologueComponent::PlayMonologue(FName MonologueID, UDataTable* MonologueTable)
{
	if (!MonologueTable || bIsPlaying) StopCurrentMonologue();
	
	FMonologueData* MonologueData = MonologueTable->FindRow<FMonologueData>(MonologueID, TEXT(""));
	if (!MonologueData) return;
	
	CurrentMonologueID = MonologueID;
	bIsPlaying = true;
	CurrentSubtitleIndex = 0;
	CurrentSubtitles = MonologueData->Subtitles;
	
	OnMonologueStarted.Broadcast(CurrentMonologueID);
	StartSubtitleSequence();
}

void UUMonologueComponent::StopCurrentMonologue()
{
	if (!bIsPlaying) return;
	
	bIsPlaying = false;
	CurrentMonologueID = NAME_None;
	CurrentSubtitles.Empty();
	CurrentSubtitleIndex = 0;
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().ClearTimer(SubtitleTimerHandle);
		GetWorld()->GetTimerManager().ClearTimer(HideTimerHandle);
	}
	
	if (AudioComponent && AudioComponent->IsPlaying())
	{
		AudioComponent->Stop();
	}
	
	if (ActiveWidget)
	{
		ActiveWidget->HideWidget();
		ActiveWidget = nullptr;
	}
	
	OnMonologueEnded.Broadcast(CurrentMonologueID);
}

void UUMonologueComponent::SetDisplayWidget(UMonologueDisplayWidget* Widget)
{
	ActiveWidget = Widget;
}

void UUMonologueComponent::StartSubtitleSequence()
{
	if (CurrentSubtitles.Num() > 0)
	{
		AdvanceToNextSubtitle();
	}
	else
	{
		FinishMonologue();
	}
}

void UUMonologueComponent::AdvanceToNextSubtitle()
{
	if (CurrentSubtitleIndex >= CurrentSubtitles.Num())
	{
		FinishMonologue();
		return;
	}
	
	FString CurrentText = CurrentSubtitles[CurrentSubtitleIndex];
	OnSubtitleChanged.Broadcast(CurrentMonologueID, CurrentText);
	
	// Получаем имя персонажа из DataTable
	UDataTable* MonologueTable = nullptr; // Нужно сохранить ссылку на таблицу
	FString CharacterName = "NPC"; // Дефолтное значение
	
	if (ActiveWidget) ActiveWidget->AnimateText(CurrentText, CharacterName);
	
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			SubtitleTimerHandle,
			this,
			&UUMonologueComponent::AdvanceToNextSubtitle,
			3.0f,
			false
		);
	}
	
	CurrentSubtitleIndex++;
}

void UUMonologueComponent::FinishMonologue()
{
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimer(
			HideTimerHandle,
			this,
			&UUMonologueComponent::HandleAudioFinished,
			2.0f,
			false
		);
	}
}

void UUMonologueComponent::HandleAudioFinished()
{
	StopCurrentMonologue();
}
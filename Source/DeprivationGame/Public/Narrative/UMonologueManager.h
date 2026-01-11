#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "Data/FMonologueData.h"
#include "UI/MonologueDisplayWidget.h"
#include "UMonologueManager.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonologueStarted, FName, MonologueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonologueEnded, FName, MonologueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubtitleChanged, FName, MonologueID, const FString&, CurrentSubtitle);

UCLASS()
class DEPRIVATIONGAME_API UUMonologueManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void PlayMonologue(FName MonologueID, UDataTable* MonologueTable);

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void StopCurrentMonologue();

	UFUNCTION(BlueprintPure, Category = "Monologue")
	bool IsPlayingMonologue() const { return bIsPlaying; }

	UFUNCTION(BlueprintPure, Category = "Monologue")
	FName GetCurrentMonologueID() const { return CurrentMonologueID; }

	UPROPERTY(BlueprintAssignable, Category = "Monologue Events")
	FOnMonologueStarted OnMonologueStarted;

	UPROPERTY(BlueprintAssignable, Category = "Monologue Events")
	FOnMonologueEnded OnMonologueEnded;

	UPROPERTY(BlueprintAssignable, Category = "Monologue Events")
	FOnSubtitleChanged OnSubtitleChanged;

private:
	UPROPERTY()
	UAudioComponent* AudioComponent;

	UPROPERTY()
	TSubclassOf<UMonologueDisplayWidget> MonologueWidgetClass;

	UPROPERTY()
	UMonologueDisplayWidget* ActiveWidget;

	bool bIsPlaying = false;
	FName CurrentMonologueID;
	FTimerHandle SubtitleTimerHandle;
	FTimerHandle HideTimerHandle;
	int32 CurrentSubtitleIndex = 0;
	TArray<FString> CurrentSubtitles;

	void StartSubtitleSequence();
	void AdvanceToNextSubtitle();
	void FinishMonologue();
	void HandleAudioFinished();
	void CreateDisplayWidget();
};


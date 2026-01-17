#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Engine/DataTable.h"
#include "Components/AudioComponent.h"
#include "Data/FMonologueData.h"
#include "UI/MonologueDisplayWidget.h"
#include "UMonologueComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonologueStarted, FName, MonologueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMonologueEnded, FName, MonologueID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSubtitleChanged, FName, MonologueID, const FString&, CurrentSubtitle);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DEPRIVATIONGAME_API UUMonologueComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UUMonologueComponent();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void PlayMonologue(FName MonologueID, UDataTable* MonologueTable);

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void StopCurrentMonologue();

	UFUNCTION(BlueprintCallable, Category = "Monologue")
	void SetDisplayWidget(UMonologueDisplayWidget* Widget);

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
};
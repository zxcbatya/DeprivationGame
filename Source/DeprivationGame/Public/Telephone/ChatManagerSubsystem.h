// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DeprivationGame/Public/Data/ChatData.h"
#include "ChatManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatUpdated, int32, ContactID);

UCLASS()
class DEPRIVATIONGAME_API UChatManagerSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnChatUpdated OnChatUpdated;

private:
	TMap<int32, FChatData> Chats;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	UFUNCTION(BlueprintCallable, Category="Chat")
	void AddMessage(int32 ContactID, const FMessageData& Message);

	UFUNCTION(BlueprintCallable, Category="Chat", BlueprintPure)
	FChatData GetChatByContactID(int32 ContactID) const;

	UFUNCTION(BlueprintCallable, Category="Chat")
	bool MarkChatAsRead(int32 ContactID);

	UFUNCTION(BlueprintCallable, Category="Chat", BlueprintPure)
	TArray<FChatData> GetAllChats() const;
};
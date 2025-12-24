// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Data/ChatData.h"
#include "ChatManagerSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatUpdated, int32, ContactID);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnMessageReceived, int32, ContactID, FMessageData, Message);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChatListRefreshRequested, int32, ContactID);

UCLASS()
class DEPRIVATIONGAME_API UChatManagerSubsystem : public UGameInstanceSubsystem
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnChatUpdated OnChatUpdated;

    UPROPERTY(BlueprintAssignable)
    FOnMessageReceived OnMessageReceived;

    UPROPERTY(BlueprintAssignable, Category = "Chat")
    FOnChatListRefreshRequested OnChatListRefreshRequested;
    
    UFUNCTION(BlueprintCallable, Category="Chat")
    void RequestChatListRefresh(int32 ContactID);

    UFUNCTION(BlueprintCallable, Category="Chat")
    void AddMessage(int32 ContactID, const FMessageData& Message);

    UFUNCTION(BlueprintCallable, Category="Chat")
    void SendMessage(int32 ContactID, const FString& SenderID, const FString& Content, const FString& SenderIconPath = "");

    UFUNCTION(BlueprintCallable, Category="Chat")
    void AddContact(int32 ContactID, const FText& ContactName, const FString& ContactIconPath = "");

    UFUNCTION(BlueprintCallable, Category="Chat")
    void CreateContact(int32 ContactID, const FText& ContactName, const FLinearColor& ContactIconColor, const FString ContactInitial);

    UFUNCTION(BlueprintCallable, Category="Chat", BlueprintPure)
    FChatData GetChatByContactID(int32 ContactID) const;

    UFUNCTION(BlueprintCallable, Category="Chat")
    bool MarkChatAsRead(int32 ContactID);

    UFUNCTION(BlueprintCallable, Category="Chat", BlueprintPure)
    TArray<FChatData> GetAllChats() const;
    
    UFUNCTION(BlueprintCallable, Category="Chat")
    void OnChatItemClicked(int32 ContactID);

protected:
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;

private:
    TMap<int32, FChatData> Chats;
};
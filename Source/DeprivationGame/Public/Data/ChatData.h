#pragma once

#include "CoreMinimal.h"
#include "ChatData.generated.h"

USTRUCT(BlueprintType)
struct FMessageData
{
    GENERATED_BODY()

public:
    FString SenderID;
    FString Content;
    FDateTime Timestamp;
    bool bIsUnread;

    FMessageData()
        : SenderID("")
        , Content("")
        , Timestamp(FDateTime::Now())
        , bIsUnread(true)
    {
    }

    FMessageData(const FString& InSenderID, const FString& InContent)
        : SenderID(InSenderID)
        , Content(InContent)
        , Timestamp(FDateTime::Now())
        , bIsUnread(true)
    {
    }
};

USTRUCT(BlueprintType)
struct FChatData
{
    GENERATED_BODY()

public:
    int32 ChatId;
    FString ContactName;
    FString LastMessage;
    FDateTime LastMessageTime;
    FLinearColor ContactIconColor;
    FString ContactInitial;
    TArray<FMessageData> Messages;
    int32 UnreadCount;

    FChatData()
        : ChatId(0)
        , ContactName("")
        , LastMessage("")
        , LastMessageTime(FDateTime::Now())
        , ContactIconColor(FLinearColor::White)
        , ContactInitial("")
        , UnreadCount(0)
    {
    }

    FChatData(int32 InChatId, const FString& InContactName, const FString& InLastMessage)
        : ChatId(InChatId)
        , ContactName(InContactName)
        , LastMessage(InLastMessage)
        , LastMessageTime(FDateTime::Now())
        , ContactIconColor(FLinearColor::White)
        , ContactInitial(InContactName.Left(1).ToUpper())
        , UnreadCount(0)
    {
    }
};
#pragma once

#include "CoreMinimal.h"
#include "ChatData.generated.h"

USTRUCT(BlueprintType)
struct FMessageData
{
    GENERATED_BODY()

public:
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Data")
    FString SenderID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Data")
    FString Content;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Data")
    FDateTime Timestamp;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Message Data")
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
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    int32 ChatId;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    FString ContactName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    FString LastMessage;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    FDateTime LastMessageTime;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    FLinearColor ContactIconColor;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    FString ContactInitial;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
    TArray<FMessageData> Messages;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Chat Data")
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
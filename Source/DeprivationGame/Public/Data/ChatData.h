#pragma once

#include "CoreMinimal.h"
#include "ChatData.generated.h"

USTRUCT(BlueprintType)
struct FMessageData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	FString SenderID;

	UPROPERTY(BlueprintReadWrite)
	FString Content;

	UPROPERTY(BlueprintReadWrite)
	FDateTime Timestamp;

	UPROPERTY(BlueprintReadWrite)
	bool bIsRead;
};

USTRUCT(BlueprintType)
struct FChatData
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite)
	int32 ContactID;

	UPROPERTY(BlueprintReadWrite)
	FText ContactName;

	UPROPERTY(BlueprintReadWrite)
	TArray<FMessageData> Messages;

	UPROPERTY(BlueprintReadWrite)
	FDateTime LastMessageTimestamp;

	UPROPERTY(BlueprintReadWrite)
	int32 UnreadCount;

	UPROPERTY(BlueprintReadWrite)
	bool bIsActive;
};
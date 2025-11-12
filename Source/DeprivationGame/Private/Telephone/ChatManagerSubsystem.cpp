// Fill out your copyright notice in the Description page of Project Settings.

#include "DeprivationGame/Public/Telephone/ChatManagerSubsystem.h"

void UChatManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// Initialize with some test data
	FChatData TestChat;
	TestChat.ContactID = 1;
	TestChat.ContactName = FText::FromString("Test Contact");
	TestChat.UnreadCount = 0;
	TestChat.bIsActive = true;
	
	FMessageData TestMessage;
	TestMessage.SenderID = "1";
	TestMessage.Content = "Hello, this is a test message!";
	TestMessage.Timestamp = FDateTime::Now();
	TestMessage.bIsRead = false;
	
	TestChat.Messages.Add(TestMessage);
	Chats.Add(1, TestChat);
}

void UChatManagerSubsystem::AddMessage(int32 ContactID, const FMessageData& Message)
{
	if (FChatData* Chat = Chats.Find(ContactID))
	{
		Chat->Messages.Add(Message);
		Chat->LastMessageTimestamp = Message.Timestamp;
		Chat->UnreadCount++;
		OnChatUpdated.Broadcast(ContactID);
	}
}

FChatData UChatManagerSubsystem::GetChatByContactID(int32 ContactID) const
{
	if (const FChatData* Chat = Chats.Find(ContactID))
	{
		return *Chat;
	}
	return FChatData();
}

bool UChatManagerSubsystem::MarkChatAsRead(int32 ContactID)
{
	if (FChatData* Chat = Chats.Find(ContactID))
	{
		Chat->UnreadCount = 0;
		for (auto& Message : Chat->Messages)
		{
			Message.bIsRead = true;
		}
		OnChatUpdated.Broadcast(ContactID);
		return true;
	}
	return false;
}

TArray<FChatData> UChatManagerSubsystem::GetAllChats() const
{
	TArray<FChatData> Result;
	Chats.GenerateValueArray(Result);
	return Result;
}
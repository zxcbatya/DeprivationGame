// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "Algo/Sort.h"

void UChatManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Chats.Empty();

	FChatData TestChat;
	TestChat.ChatId = 1000;
	TestChat.ContactName = "Test Contact";
	TestChat.LastMessage = "Hello, this is a test message!";
	TestChat.UnreadCount = 0;
	TestChat.LastMessageTime = FDateTime::Now();

	FMessageData TestMessage;
	TestMessage.SenderID = "1000";
	TestMessage.Content = "Hello, this is a test message!";
	TestMessage.Timestamp = FDateTime::Now();
	TestMessage.bIsUnread = true;

	TestChat.Messages.Add(TestMessage);
	Chats.Add(1000, TestChat);
}


void UChatManagerSubsystem::RequestChatListRefresh(int32 ContactID)
{
	OnChatListRefreshRequested.Broadcast(ContactID);
}

void UChatManagerSubsystem::AddMessage(int32 ContactID, const FMessageData& Message)
{
	FChatData& Chat = Chats[ContactID];
	Chat.Messages.Add(Message);

	if (Message.bIsUnread)
		++Chat.UnreadCount;

	Chat.LastMessageTime = Message.Timestamp;
	Chat.LastMessage = Message.Content;
	OnChatUpdated.Broadcast(ContactID);
	OnMessageReceived.Broadcast(ContactID, Message);
}

void UChatManagerSubsystem::SendMessage(int32 ContactID, const FString& SenderID, const FString& Content,
                                        const FString& SenderIconPath)
{
	FMessageData Message;
	Message.SenderID = SenderID;
	Message.Content = Content;
	Message.Timestamp = FDateTime::Now();
	Message.bIsUnread = true;

	AddMessage(ContactID, Message);
}

void UChatManagerSubsystem::AddContact(int32 ContactID, const FText& ContactName, const FString& ContactIconPath)
{
	if (!Chats.Contains(ContactID))
	{
		FChatData NewChat;
		NewChat.ChatId = ContactID;
		NewChat.ContactName = ContactName.ToString();
		NewChat.UnreadCount = 0;
		NewChat.LastMessageTime = FDateTime::Now();
		Chats.Add(ContactID, NewChat);
		OnChatUpdated.Broadcast(ContactID);
	}
}

void UChatManagerSubsystem::CreateContact(int32 ContactID, const FText& ContactName,
                                          const FLinearColor& ContactIconColor, const FString ContactInitial)
{
	if (!Chats.Contains(ContactID))
	{
		FChatData NewChat;
		NewChat.ChatId = ContactID;
		NewChat.ContactName = ContactName.ToString();
		NewChat.UnreadCount = 0;
		NewChat.LastMessageTime = FDateTime::Now();
		NewChat.ContactIconColor = ContactIconColor;
		NewChat.ContactInitial = ContactInitial;
		Chats.Add(ContactID, NewChat);
		OnChatUpdated.Broadcast(ContactID);
	}
	else
	{
		FChatData& Chat = Chats[ContactID];
		Chat.ContactName = ContactName.ToString();
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
		for (FMessageData& Msg : Chat->Messages)
			Msg.bIsUnread = false;

		Chat->UnreadCount = 0;
		OnChatUpdated.Broadcast(ContactID);
		return true;
	}

	return false;
}

TArray<FChatData> UChatManagerSubsystem::GetAllChats() const
{
	TArray<FChatData> Result;
	for (const auto& Pair : Chats)
	{
		Result.Add(Pair.Value);
	}

	Algo::SortBy(Result, [](const FChatData& Chat)
	{
		return Chat.LastMessageTime;
	}, TGreater<FDateTime>());

	return Result;
}

void UChatManagerSubsystem::OnChatItemClicked(int32 ContactID)
{
	UE_LOG(LogTemp, Warning, TEXT("Chat item clicked, ContactID: %d"), ContactID);
}

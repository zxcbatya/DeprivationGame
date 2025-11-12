// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/ChatManagerSubsystem.h"

void UChatManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	Chats.Empty();
}

void UChatManagerSubsystem::AddMessage(int32 ContactID, const FMessageData& Message)
{
	FChatData* Chat = Chats.Find(ContactID);
	if (!Chat)
	{
		FChatData New;
		New.ContactID = ContactID;
		Chats.Add(ContactID, New);
		Chat = &Chats.FindChecked(ContactID);
	}
	Chat->Messages.Add(Message);
	if (!Message.bIsRead) ++Chat->UnreadCount;
	Chat->LastMessageTimestamp = Message.Timestamp;
	OnChatUpdated.Broadcast(ContactID);
}

FChatData UChatManagerSubsystem::GetChatByContactID(int32 ContactID) const
{
	const FChatData* Chat = Chats.Find(ContactID);
	return Chat ? *Chat : FChatData();
}

bool UChatManagerSubsystem::MarkChatAsRead(int32 ContactID)
{
	FChatData* Chat = Chats.Find(ContactID);
	if (!Chat) return false;
	for (FMessageData& Msg : Chat->Messages)
		Msg.bIsRead = true;
	Chat->UnreadCount = 0;
	OnChatUpdated.Broadcast(ContactID);
	return true;
}

TArray<FChatData> UChatManagerSubsystem::GetAllChats() const
{
	TArray<FChatData> Result;
	for (const auto& Pair : Chats)
		Result.Add(Pair.Value);
	// Optional sort: Algo::Sort(Result, { return A.LastMessageTimestamp > B.LastMessageTimestamp; });
	return Result;
}
// Fill out your copyright notice in the Description page of Project Settings.

#include "Telephone/ChatManagerSubsystem.h"
#include "Data/ChatData.h"
#include "Algo/Sort.h"

void UChatManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    Chats.Empty();
    
    // Initialize with some test data
    FChatData TestChat;
    TestChat.ChatId = 1000; // Изменили ID, чтобы не конфликтовать с пользовательскими контактами
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
    Chats.Add(1000, TestChat); // Изменили ID
    
    UE_LOG(LogTemp, Warning, TEXT("ChatManagerSubsystem initialized with test contact ID: 1000"));
}


void UChatManagerSubsystem::RequestChatListRefresh(int32 ContactID)
{
    OnChatListRefreshRequested.Broadcast(ContactID);
}

void UChatManagerSubsystem::AddMessage(int32 ContactID, const FMessageData& Message)
{
    UE_LOG(LogTemp, Warning, TEXT("AddMessage called for ContactID: %d"), ContactID);
    
    if (!Chats.Contains(ContactID))
    {
        FChatData NewChat;
        NewChat.ChatId = ContactID;
        NewChat.LastMessageTime = Message.Timestamp;
        Chats.Add(ContactID, NewChat);
        UE_LOG(LogTemp, Warning, TEXT("Created new chat for ContactID: %d"), ContactID);
    }
    
    FChatData& Chat = Chats[ContactID];
    Chat.Messages.Add(Message);
    
    if (Message.bIsUnread) 
        ++Chat.UnreadCount;
        
    Chat.LastMessageTime = Message.Timestamp;
    Chat.LastMessage = Message.Content;
    OnChatUpdated.Broadcast(ContactID);
    OnMessageReceived.Broadcast(ContactID, Message);
}

void UChatManagerSubsystem::SendMessage(int32 ContactID, const FString& SenderID, const FString& Content, const FString& SenderIconPath)
{
    UE_LOG(LogTemp, Warning, TEXT("SendMessage called for ContactID: %d, SenderID: %s"), ContactID, *SenderID);
    
    FMessageData Message;
    Message.SenderID = SenderID;
    Message.Content = Content;
    Message.Timestamp = FDateTime::Now();
    Message.bIsUnread = true;
    
    AddMessage(ContactID, Message);
}

void UChatManagerSubsystem::AddContact(int32 ContactID, const FText& ContactName, const FString& ContactIconPath)
{
    UE_LOG(LogTemp, Warning, TEXT("AddContact called for ContactID: %d, Name: %s"), ContactID, *ContactName.ToString());
    
    if (!Chats.Contains(ContactID))
    {
        FChatData NewChat;
        NewChat.ChatId = ContactID;
        NewChat.ContactName = ContactName.ToString();
        NewChat.UnreadCount = 0;
        NewChat.LastMessageTime = FDateTime::Now();
        Chats.Add(ContactID, NewChat);
        OnChatUpdated.Broadcast(ContactID);
        UE_LOG(LogTemp, Warning, TEXT("Added new contact ID: %d, Name: %s"), ContactID, *ContactName.ToString());
    }
}

void UChatManagerSubsystem::CreateContact(int32 ContactID, const FText& ContactName, const FLinearColor& ContactIconColor, const FString ContactInitial)
{
    UE_LOG(LogTemp, Warning, TEXT("CreateContact called for ContactID: %d, Name: %s"), ContactID, *ContactName.ToString());
    
    if (!Chats.Contains(ContactID))
    {
        FChatData NewChat;
        NewChat.ChatId = ContactID;
        NewChat.ContactName = ContactName.ToString();
        NewChat.UnreadCount = 0;
        NewChat.LastMessageTime = FDateTime::Now();
        NewChat.ContactIconColor =  ContactIconColor;
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
    UE_LOG(LogTemp, Warning, TEXT("GetChatByContactID called for ContactID: %d"), ContactID);
    
    if (const FChatData* Chat = Chats.Find(ContactID))
    {
        UE_LOG(LogTemp, Warning, TEXT("Found chat for ContactID: %d, Name: %s"), ContactID, *Chat->ContactName);
        return *Chat;
    }
        
    UE_LOG(LogTemp, Warning, TEXT("Chat not found for ContactID: %d"), ContactID);
    return FChatData();
}

bool UChatManagerSubsystem::MarkChatAsRead(int32 ContactID)
{
    UE_LOG(LogTemp, Warning, TEXT("MarkChatAsRead called for ContactID: %d"), ContactID);
    
    if (FChatData* Chat = Chats.Find(ContactID))
    {
        for (FMessageData& Msg : Chat->Messages)
            Msg.bIsUnread = false;
            
        Chat->UnreadCount = 0;
        OnChatUpdated.Broadcast(ContactID);
        UE_LOG(LogTemp, Warning, TEXT("Marked chat as read for ContactID: %d"), ContactID);
        return true;
    }
    
    UE_LOG(LogTemp, Warning, TEXT("Chat not found for MarkChatAsRead, ContactID: %d"), ContactID);
    return false;
}

TArray<FChatData> UChatManagerSubsystem::GetAllChats() const
{
    UE_LOG(LogTemp, Warning, TEXT("GetAllChats called, found %d chats"), Chats.Num());
    
    TArray<FChatData> Result;
    for (const auto& Pair : Chats)
    {
        Result.Add(Pair.Value);
        UE_LOG(LogTemp, Warning, TEXT("Adding chat to result: ID: %d, Name: %s"), Pair.Value.ChatId, *Pair.Value.ContactName);
    }
        
    Algo::SortBy(Result, [](const FChatData& Chat) {
        return Chat.LastMessageTime;
    }, TGreater<FDateTime>());
    
    return Result;
}

void UChatManagerSubsystem::OnChatItemClicked(int32 ContactID)
{
    // Handle chat item click
    UE_LOG(LogTemp, Warning, TEXT("Chat item clicked, ContactID: %d"), ContactID);
    // Add your chat opening logic here
}
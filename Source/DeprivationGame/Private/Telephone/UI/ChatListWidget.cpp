// Fill out your copyright notice in the Description page of Project Settings.


#include "DeprivationGame/Public/Telephone/UI/ChatListWidget.h"
#include "DeprivationGame/Public/Telephone/ChatManagerSubsystem.h"
#include "DeprivationGame/Public/Data/ChatData.h"

void UChatListWidget::NativeConstruct()
{
    Super::NativeConstruct();
    
    UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
    if (Mgr)
    {
        Mgr->OnChatUpdated.AddDynamic(this, &UChatListWidget::RefreshChatList);
    }
    
    // Вызовем RefreshChatList с временным значением, так как у нас нет ContactID на старте
    RefreshChatList(-1);
}

void UChatListWidget::RefreshChatList(int32 ContactID)
{
    // ContactID передается, но в данном случае нам не нужен
    // Можно использовать его или игнорировать
    
    UChatManagerSubsystem* Mgr = GetGameInstance()->GetSubsystem<UChatManagerSubsystem>();
    if (!Mgr)
        return;
        
    TArray<FChatData> Chats = Mgr->GetAllChats();
    ChatListView->ClearListItems();
    
    for (const auto& C : Chats)
    {
        // Note: You'll need to create and assign MessageEntryClass
        // UMessageEntryWidget* Item = CreateWidget<UMessageEntryWidget>(this, MessageEntryClass);
        // Item->Setup(C);
        // ChatListView->AddItem(Item);
        
        // Placeholder implementation:
        UObject* Item = NewObject<UObject>(this);
        ChatListView->AddItem(Item);
    }
}